// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UHealthComponent;
class AWeapon;



UCLASS()
class SHOOTINGGAME_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ABaseCharacter();

protected:
	
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;


public:	

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void HealPlayerHealth(int32 HealAmount);

	AWeapon* GetCurrentWeapon() const;

protected:

	void UpDown(float Value);
	void LeftRight(float Value);
	void Yaw(float Value);
	void Pitch(float Value);

	void BeginCrouch();
	void EndCrouch();

	void BeginZoom();
	void EndZoom();

	void SetTargeting(bool IsTargeting);
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetTargeting(bool IsTargeting);
	void ServerSetTargeting_Implementation(bool IsTargeting);
	bool ServerSetTargeting_Validate(bool IsTargeting);

	UFUNCTION(BlueprintCallable, Category = "Player")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Player")
	void StopFire();

	UFUNCTION()
	void OnHealthChanged(float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void OnDeath();

	void Reload();


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* SpringArmComp;


	bool bWantsToZoom;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float ZoomedFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100))
	float ZoomInterpSpeed;

	float DefaultFOV;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeapon> WeaponClass;




public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		TWeakObjectPtr<class UHealthComponent> HealthComp;
		//UHealthComponent* HealthComp;

	UPROPERTY(Transient, Replicated)
	bool bIsTargeting;

	UPROPERTY(Transient, Replicated)
	bool bIsDeath;

	UPROPERTY(Transient,Replicated)
	AWeapon* CurrentWeapon;

};
