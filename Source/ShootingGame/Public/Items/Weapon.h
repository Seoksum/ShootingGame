// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UParticleSystem;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoInClipChanged, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTotalAmmoChanged, int32);

UENUM()
enum class EWeaponState
{
	Idle,
	Firing,
	Reloading
};

// weapon linetrace의 정보 
USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FHitResult Hit;

	UPROPERTY()
	bool bPlayerHit;

	UPROPERTY()
	FVector_NetQuantize TraceTo;
};

UCLASS()
class SHOOTINGGAME_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();

	void SetOwningPawn(class ABaseCharacter* NewOwner);


protected:

	UPROPERTY(Transient, ReplicatedUsing = OnRep_MyPawn)
		class ABaseCharacter* MyPawn;

	UFUNCTION()
	void OnRep_MyPawn();

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* MeshComp;

	void PlayFireEffects(FVector TraceEnd);

	void PlayImpactEffects(const FHitResult& Hit, bool bPlayerHit, FVector ImpactPoint);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<UCameraShakeBase> FireCamShake;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float BaseDamage;

	void HandleFire();

	void Fire();
	UFUNCTION(Reliable, Server, WithValidation)
		void ServerFire();
	void ServerFire_Implementation();
	bool ServerFire_Validate();



	FTimerHandle TimerHandle_TimeBetweenShots;

	float LastFiredTime;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RateOfFire;

	float TimeBetweenShots;

	// 이 속성이 Replicate될 때마다 함수를 트리거 
	UPROPERTY(ReplicatedUsing = OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;

	UFUNCTION()
	void OnRep_HitScanTrace();


	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayWeaponAnimation(UAnimMontage* Animation);
	void MulticastPlayWeaponAnimation_Implementation(UAnimMontage* Animation);

	void PlayWeaponSound(USoundBase* SoundToPlay);

	void DecalScan(const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category = "Decal")
	UMaterial* DecalMaterial;


public:

	bool CanFire();

	void StartFire();
	void StopFire();


	bool CanReload();

	void StartReload();
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerStartReload();
	bool ServerStartReload_Validate();
	
	void StopSimulateReload();

	void ReloadWeapon();


	void SetCurrentAmmoCount(int32 AmmoDelta);
	UFUNCTION(Server, Reliable)
		void ServerSetCurrentAmmoCount(int32 AmmoDelta);

	void UseAmmo();

	int32 GetCurrentAmmo();

	int32 GetCurrentAmmoInClip() const;

	void AddAmmoCount(int32 ExtraAmount);
	
	UFUNCTION()
	void OnRep_TotalAmmoCount();

	FTimerHandle TimerHandle_StopReload;
	FTimerHandle TimerHandle_HandleFiring;


	UPROPERTY(EditDefaultsOnly, Category = "Animations")
		UAnimMontage* ReloadAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sounds")
		USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sounds")
		USoundBase* ReloadSound;

public:

	EWeaponState GetCurrentState() const;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_Reload)
	bool bIsReloading;

	UFUNCTION()
	void OnRep_Reload();

	FOnAmmoInClipChanged OnAmmoInClipChanged;

	FOnTotalAmmoChanged OnTotalAmmoChanged;

private:

	UPROPERTY(ReplicatedUsing = OnRep_TotalAmmoCount)
		int32 CurrentAmmo;

	UPROPERTY(EditDefaultsOnly)
		int32 CurrentAmmoInClip;

	UPROPERTY(EditDefaultsOnly)
		int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly)
		int32 MaxAmmoPerClip;

	EWeaponState CurrentState;


};
