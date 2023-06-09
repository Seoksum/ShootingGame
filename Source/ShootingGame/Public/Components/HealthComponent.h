// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnHealthChanged, float, Health, float, HealthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTINGGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool bIsDead;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "HealthComponent")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent")
	float DefaultHealth;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "HealthComponent")
	float HpRatio;


	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);


public:

	float GetHpRatio();

	FOnHealthChanged OnHealthChanged;

	void Heal(float Amount);


};
