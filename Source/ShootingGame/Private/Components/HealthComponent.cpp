// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	Health = 100;
	DefaultHealth = 100;
	bIsDead = false;
	HpRatio = 1.f;

	SetIsReplicatedByDefault(true);
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	//if (GetOwnerRole() == ROLE_Authority) // ActorComponentÀÇ GetOwner

	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	}

	Health = DefaultHealth;
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || bIsDead)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	HpRatio = Health / DefaultHealth;
	bIsDead = Health <= 0.0f;
	OnHealthChanged.Broadcast(Health, Damage, DamageType, InstigatedBy, DamageCauser);
}

void UHealthComponent::Heal(float Amount)
{
	if (Amount <= 0.f || bIsDead)
	{
		return;
	}
	//	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health));
	Health = FMath::Clamp(Health + Amount, 0.f, DefaultHealth);
	HpRatio = Health / DefaultHealth;
}

float UHealthComponent::GetHpRatio()
{
	return HpRatio;
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, Health);
	DOREPLIFETIME(UHealthComponent, HpRatio);

}
