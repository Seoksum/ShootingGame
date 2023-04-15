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

	/* RPM - Bullets per minute fired by weapon */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float RateOfFire;

	/* Bullet Spread in Degrees */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin = 0.0f))
		float BulletSpread;

	// Derived from RateOfFire
	float TimeBetweenShots;

	// 이 속성이 Replicate될 때마다 함수를 트리거 
	UPROPERTY(ReplicatedUsing = OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;

	UFUNCTION()
	void OnRep_HitScanTrace();

	UPROPERTY(ReplicatedUsing = OnRep_TotalAmmoCount)
	int32 CurrentAmmo;

	UPROPERTY(EditDefaultsOnly)
	int32 CurrentAmmoInClip;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxAmmoPerClip;


	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayWeaponAnimation(UAnimMontage* Animation);
	void MulticastPlayWeaponAnimation_Implementation(UAnimMontage* Animation);

	void PlayWeaponSound(USoundBase* SoundToPlay);

	void DecalScan(const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category = "Decal")
	UMaterial* DecalMaterial;


public:

	void StartFire();
	UFUNCTION(reliable, Server, WithValidation)
		void ServerStartFire();
	void ServerStartFire_Implementation();
	bool ServerStartFire_Validate();

	void StopFire();
	UFUNCTION(Reliable, Server, WithValidation)
		void ServerStopFire();
	void ServerStopFire_Implementation();
	bool ServerStopFire_Validate();

	void ReloadWeapon();

	void SetCurrentAmmoCount(int32 AmmoDelta);

	UFUNCTION(Server, Reliable)
	void ServerSetCurrentAmmoCount(int32 AmmoDelta);
	

	/* Is weapon and character currently capable of starting a reload */
	bool CanReload();

	void StartReload();

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerStartReload();
	void ServerStartReload_Implementation();
	bool ServerStartReload_Validate();
	
	void StopSimulateReload();

	UFUNCTION(Reliable, server, WithValidation)
	void ServerStopReload();
	void ServerStopReload_Implementation();
	bool ServerStopReload_Validate();

	void UseAmmo();

	int32 GetCurrentAmmo();

	int32 GetCurrentAmmoInClip() const;

	void AddAmmoCount(int32 ExtraAmount);
	
	UFUNCTION()
	void OnRep_TotalAmmoCount();

	FTimerHandle TimerHandle_StopReload;
	FTimerHandle TimerHandle_HandleFiring;


	UPROPERTY(EditDefaultsOnly, Category = "Animations")
		UAnimMontage* FireAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
		UAnimMontage* ReloadAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sounds")
		USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sounds")
		USoundBase* ReloadSound;

public:

	bool CanFire();

	EWeaponState GetCurrentState() const;

	EWeaponState CurrentState;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_Reload)
	bool bIsReloading;

	UFUNCTION()
	void OnRep_Reload();


	FOnAmmoInClipChanged OnAmmoInClipChanged;
	FOnTotalAmmoChanged OnTotalAmmoChanged;
};
