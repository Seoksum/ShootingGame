// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFrameworks/MyGameModeBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Items/Weapon.h"
#include "Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/ShootingPlayerController.h"
#include "GameFrameworks/ShootingGameState.h"



// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

	ZoomedFOV = 65.0f;
	ZoomInterpSpeed = 20;


}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = CameraComp->FieldOfView;

}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	HealthComp->OnHealthChanged.AddDynamic(this, &ABaseCharacter::OnHealthChanged);

	if (HasAuthority())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		CurrentWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (CurrentWeapon)
		{
			CurrentWeapon->SetOwningPawn(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));
		}
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;
	float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);

	CameraComp->SetFieldOfView(NewFOV);

}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("UpDown", this, &ABaseCharacter::UpDown);
	PlayerInputComponent->BindAxis("LeftRight", this, &ABaseCharacter::LeftRight);
	PlayerInputComponent->BindAxis("Turn", this, &ABaseCharacter::Yaw);
	PlayerInputComponent->BindAxis("LookUp", this, &ABaseCharacter::Pitch);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABaseCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ABaseCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ABaseCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ABaseCharacter::EndZoom);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABaseCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABaseCharacter::StopFire);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ABaseCharacter::Reload);
}

void ABaseCharacter::HealPlayerHealth(int32 HealAmount)
{
	HealthComp->Heal(HealAmount);
}

AWeapon* ABaseCharacter::GetCurrentWeapon() const
{
	if (CurrentWeapon)
	{
		return CurrentWeapon;
	}
	return nullptr;
}

void ABaseCharacter::UpDown(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void ABaseCharacter::LeftRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}
void ABaseCharacter::Yaw(float Value)
{
	AddControllerYawInput(Value);
}

void ABaseCharacter::Pitch(float Value)
{
	AddControllerPitchInput(Value);
}

void ABaseCharacter::BeginCrouch()
{
	Crouch();
}

void ABaseCharacter::EndCrouch()
{
	UnCrouch();
}

void ABaseCharacter::BeginZoom()
{
	bWantsToZoom = true;
	SetTargeting(true);
}

void ABaseCharacter::EndZoom()
{
	bWantsToZoom = false;
	SetTargeting(false);
}

void ABaseCharacter::SetTargeting(bool IsTargeting)
{
	if (!HasAuthority())
	{
		ServerSetTargeting(IsTargeting);
	}
	bIsTargeting = IsTargeting;
}

void ABaseCharacter::ServerSetTargeting_Implementation(bool IsTargeting)
{
	SetTargeting(IsTargeting);
}

bool ABaseCharacter::ServerSetTargeting_Validate(bool IsTargeting)
{
	return true;
}

void ABaseCharacter::StartFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}
}

void ABaseCharacter::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void ABaseCharacter::OnHealthChanged(float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bIsDeath)
	{
		bIsDeath = true;
		OnDeath();
	}
}

void ABaseCharacter::OnDeath()
{
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->SetComponentTickEnabled(false);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

	DetachFromControllerPendingDestroy();

	TearOff(); // 액터를 분리하여 클라이언트로의 복제를 중지한다.
	SetActorEnableCollision(true);
}

void ABaseCharacter::Reload()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartReload();
	}
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, CurrentWeapon);
	DOREPLIFETIME(ABaseCharacter, bIsDeath);
	DOREPLIFETIME_CONDITION(ABaseCharacter, bIsTargeting, COND_SkipOwner);
}





