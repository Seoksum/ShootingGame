// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PowerupActor.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/BaseCharacter.h"
#include "Components/DecalComponent.h"


// Sets default values
APowerupActor::APowerupActor()
{
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionProfileName(FName("OverlapAll"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->SetRelativeRotation(FRotator(90, 0.0f, 0.0f));
	DecalComp->DecalSize = FVector(64, 75, 75);
	DecalComp->SetupAttachment(RootComponent);

	EffectDuration = 0.f;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void APowerupActor::BeginPlay()
{
	Super::BeginPlay();

	Respawn();
}

void APowerupActor::Respawn()
{
	bIsMeshVisiblity = true;
	MeshComp->SetVisibility(bIsMeshVisiblity);
	MeshComp->SetCollisionProfileName(FName("OverlapAll"));
}

void APowerupActor::StopPowerupEffect()
{
	OnExpired();
	GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
}

void APowerupActor::OnRep_RespawnItem()
{
	MeshComp->SetVisibility(bIsMeshVisiblity);
}

void APowerupActor::StartPowerupEffect()
{
	OnActivated();
	GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &APowerupActor::StopPowerupEffect, EffectDuration, true);
}

void APowerupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (HasAuthority())
	{
		Player = Cast<ABaseCharacter>(OtherActor);
		StartPowerupEffect();
		MeshComp->SetCollisionProfileName(FName("NoCollision"));
		bIsMeshVisiblity = false;
		MeshComp->SetVisibility(bIsMeshVisiblity);
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &APowerupActor::Respawn, RespawnCoolTime);
	}
}

void APowerupActor::OnActivated()
{
}

void APowerupActor::OnExpired()
{
}

void APowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APowerupActor, bIsMeshVisiblity);

}

