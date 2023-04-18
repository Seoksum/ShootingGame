// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Weapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"
#include "Characters/BaseCharacter.h"
#include "ShootingGame/ShootingGame.h"
#include "Blueprint/UserWidget.h"
#include "UI/AmmoCountWidget.h"



AWeapon::AWeapon()
{

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionObjectType(ECC_WorldDynamic);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	MeshComp->bHiddenInGame = false;

	RootComponent = MeshComp;

	MuzzleSocketName = "MuzzleFlashSocket";
	TracerTargetName = "Target";

	BaseDamage = 10.0f;
	RateOfFire = 420;

	SetReplicates(true);

	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;

	MaxAmmo = 200;
	MaxAmmoPerClip = 20;

	CurrentState = EWeaponState::Idle;
}


void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	OnRep_MyPawn();

	OnAmmoInClipChanged.Broadcast(MaxAmmoPerClip);
	OnTotalAmmoChanged.Broadcast(MaxAmmo);
}

void AWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TimeBetweenShots = 60.f / RateOfFire;
	CurrentAmmo = MaxAmmo;
	CurrentAmmoInClip = MaxAmmoPerClip;
}

void AWeapon::HandleFire()
{
	if (CurrentAmmoInClip > 0 && CanFire()) //장전된 총알 개수가 0보다 크고 Fire할 수 있다면
	{
		//클라이언트 전용 함수 기능
		if (MyPawn && MyPawn->IsLocallyControlled())
		{
			Fire(); // 실질적으로 Fire 구현 함수
			UseAmmo(); // 장전된 총알 개수 감소
		}
	}

	else if (CanReload()) // 총알이 부족한데, 재장전이 가능하다면 
	{
		StartReload(); // 재장전
	}

	GetWorldTimerManager().SetTimer(TimerHandle_HandleFiring, this, &AWeapon::HandleFire, TimeBetweenShots, false);
	LastFiredTime = GetWorld()->GetTimeSeconds();

}


void AWeapon::Fire()
{
	if (!HasAuthority())
	{
		ServerFire();
	}

	CurrentState = EWeaponState::Firing;
	FHitResult Hit;
	FVector EyeLocation;
	FRotator EyeRotation;
	MyPawn->GetActorEyesViewPoint(EyeLocation, EyeRotation); // OUT parameter

	FVector TraceStart = MeshComp->GetSocketLocation(MuzzleSocketName);
	FVector ShotDirection = EyeRotation.Vector();
	FVector TraceEnd = TraceStart + (ShotDirection * 3000); // 종료 지점
	FVector TracerEndPoint = TraceEnd;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(MyPawn);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_GameTraceChannel1, QueryParams);
	FDamageEvent DamageEvent;

	bool bPlayerHit = false;

	if (bHit)
	{
		ABaseCharacter* Player = Cast<ABaseCharacter>(Hit.GetActor());
		if (Player)
		{
			UGameplayStatics::ApplyPointDamage(Player, BaseDamage, ShotDirection, Hit, MyPawn->GetInstigatorController(), MyPawn, DamageType);
			bPlayerHit = true;
		}
		TracerEndPoint = Hit.ImpactPoint;
	}

	if (HasAuthority())
	{
		HitScanTrace.Hit = Hit;
		HitScanTrace.bPlayerHit = bPlayerHit;
		HitScanTrace.TraceTo = TracerEndPoint;

		PlayFireEffects(TracerEndPoint);
		PlayImpactEffects(Hit, bPlayerHit, TracerEndPoint);
	}

	PlayWeaponSound(FireSound);
	LastFiredTime = GetWorld()->TimeSeconds;
}

void AWeapon::ServerFire_Implementation()
{
	Fire();
}

bool AWeapon::ServerFire_Validate()
{
	return true;
}

void AWeapon::DecalScan(const FHitResult& Hit)
{
	if (DecalMaterial)
	{
		FVector ImpactNormal = Hit.ImpactNormal;
		ImpactNormal.Normalize();
		/* Inverse to point towards the wall. Invert to get the correct orientation of the decal (pointing into the surface instead of away, messing with the normals, and lighting) */
		ImpactNormal = -ImpactNormal;

		FRotator RandomDecalRotation = ImpactNormal.ToOrientationRotator();
		RandomDecalRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);
		float DecalSize = 16.f;

		UDecalComponent* DecalComp = UGameplayStatics::SpawnDecalAttached(DecalMaterial, FVector(DecalSize, DecalSize, DecalSize),
			Hit.Component.Get(), NAME_None,
			Hit.ImpactPoint, RandomDecalRotation, EAttachLocation::KeepWorldPosition,
			10.f);

		if (DecalComp)
		{
			DecalComp->SetFadeOut(10.f, 0.5f, false);
		}
	}
}


void AWeapon::OnRep_HitScanTrace()
{
	PlayFireEffects(HitScanTrace.TraceTo);
	PlayImpactEffects(HitScanTrace.Hit, HitScanTrace.bPlayerHit, HitScanTrace.TraceTo);
}

void AWeapon::StartFire()
{
	HandleFire();

}

void AWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_HandleFiring);
}

void AWeapon::PlayFireEffects(FVector TraceEnd)
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}

	if (TracerEffect)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
		if (TracerComp)
		{
			TracerComp->SetVectorParameter(TracerTargetName, TraceEnd);
		}
	}

	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner)
	{
		APlayerController* PC = Cast<APlayerController>(MyOwner->GetController());
		if (PC)
		{
			PC->ClientStartCameraShake(FireCamShake);
		}
	}
}


void AWeapon::PlayImpactEffects(const FHitResult& Hit, bool bPlayerHit, FVector ImpactPoint)
{
	UParticleSystem* SelectedEffect = FleshImpactEffect;
	if (!bPlayerHit)
	{
		SelectedEffect = DefaultImpactEffect;
		DecalScan(Hit);
	}


	if (SelectedEffect)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		FVector ShotDirection = ImpactPoint - MuzzleLocation;
		ShotDirection.Normalize();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, ImpactPoint, ShotDirection.Rotation());
	}
}

void AWeapon::StartReload()
{
	if (!CanReload())
	{
		return; // 조기 return
	}
	if (!HasAuthority())
	{
		ServerStartReload();
	}
	
	bIsReloading = true;
	CurrentState = EWeaponState::Reloading;
	// 재장전 애니메이션 재생
	MulticastPlayWeaponAnimation(ReloadAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_StopReload, this, &AWeapon::StopSimulateReload, 1.5f, true);

	ReloadWeapon(); // CurrentAmmo 값 업데이트 
	if (MyPawn && MyPawn->IsLocallyControlled())
	{
		PlayWeaponSound(ReloadSound); // 재장전 소리 재생
	}
}

void AWeapon::ServerStartReload_Implementation()
{
	StartReload();
	MulticastPlayWeaponAnimation(ReloadAnim);
}

bool AWeapon::ServerStartReload_Validate()
{
	return true;
}

void AWeapon::StopSimulateReload()
{
	bIsReloading = false;
	MyPawn->StopAnimMontage(ReloadAnim);
	CurrentState = EWeaponState::Idle;
	GetWorldTimerManager().ClearTimer(TimerHandle_StopReload);
}

void AWeapon::ReloadWeapon()
{
	int32 AmmoDelta = FMath::Min(MaxAmmoPerClip - CurrentAmmoInClip, CurrentAmmo);
	if (AmmoDelta > 0)
	{
		CurrentAmmoInClip += AmmoDelta;
		OnAmmoInClipChanged.Broadcast(CurrentAmmoInClip);
	}
	SetCurrentAmmoCount(AmmoDelta);
}

void AWeapon::SetCurrentAmmoCount(int32 AmmoDelta)
{
	if (!HasAuthority())
	{
		ServerSetCurrentAmmoCount(AmmoDelta);
	}
	CurrentAmmo = FMath::Max(0, CurrentAmmo - AmmoDelta);
	OnRep_TotalAmmoCount();
}

void AWeapon::ServerSetCurrentAmmoCount_Implementation(int32 AmmoDelta)
{
	SetCurrentAmmoCount(AmmoDelta);
}


void AWeapon::UseAmmo()
{
	CurrentAmmoInClip--;
	OnAmmoInClipChanged.Broadcast(CurrentAmmoInClip);
}

int32 AWeapon::GetCurrentAmmo()
{
	return CurrentAmmo;
}

int32 AWeapon::GetCurrentAmmoInClip() const
{
	return CurrentAmmoInClip;
}

void AWeapon::AddAmmoCount(int32 ExtraAmount)
{
	CurrentAmmo = FMath::Min(CurrentAmmo + ExtraAmount, MaxAmmo);
	OnRep_TotalAmmoCount();
}

void AWeapon::OnRep_TotalAmmoCount()
{
	OnTotalAmmoChanged.Broadcast(CurrentAmmo);
}

bool AWeapon::CanReload()
{
	bool bIsMyPawnAlive = (!MyPawn || !MyPawn->bIsDeath);
	bool bMyState = (CurrentState == EWeaponState::Idle) || (CurrentState == EWeaponState::Firing);
	bool bAmmoCount = (CurrentAmmoInClip < MaxAmmoPerClip) && (CurrentAmmo > 0);

	return (bIsMyPawnAlive && bMyState && bAmmoCount);
}

void AWeapon::SetOwningPawn(ABaseCharacter* NewOwner)
{
	if (MyPawn != NewOwner)
	{
		SetInstigator(NewOwner);
		MyPawn = NewOwner;
		SetOwner(NewOwner);
	}
}

void AWeapon::OnRep_MyPawn()
{
	MyPawn = Cast<ABaseCharacter>(GetOwner());
	if (MyPawn)
	{
		SetOwningPawn(MyPawn);
	}
	else
	{
		SetOwningPawn(nullptr);
	}
}

bool AWeapon::CanFire()
{
	bool bIsMyPawnAlive = MyPawn && !MyPawn->bIsDeath;
	bool bMyState = (CurrentState == EWeaponState::Idle) || (CurrentState == EWeaponState::Firing);
	return bIsMyPawnAlive && bMyState && !bIsReloading;
}

void AWeapon::MulticastPlayWeaponAnimation_Implementation(UAnimMontage* Animation)
{
	if (MyPawn && Animation)
	{
		MyPawn->PlayAnimMontage(Animation);
	}
}

void AWeapon::PlayWeaponSound(USoundBase* SoundToPlay)
{
	UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, GetActorLocation());
}

EWeaponState AWeapon::GetCurrentState() const
{
	return CurrentState;
}

void AWeapon::OnRep_Reload()
{
	if (bIsReloading)
	{
		StartReload();
	}
	else
	{
		StopSimulateReload();
	}
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, HitScanTrace);

	DOREPLIFETIME(AWeapon, MyPawn);
	DOREPLIFETIME(AWeapon, CurrentAmmo);
	DOREPLIFETIME_CONDITION(AWeapon, bIsReloading, COND_SkipOwner);
}