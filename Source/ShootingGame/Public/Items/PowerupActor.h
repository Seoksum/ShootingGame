// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerupActor.generated.h"


UCLASS()
class SHOOTINGGAME_API APowerupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerupActor();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UDecalComponent* DecalComp;

	// ������ ȿ�� ���� �ð�
	UPROPERTY(EditDefaultsOnly,Category="Powerups")
	float EffectDuration;

	FTimerHandle TimerHandle_PowerupTick;

	// ������ Mesh�� SetVisibility ����� Ŭ���̾�Ʈ���Ե� �����ϱ� ���� ����
	UPROPERTY(ReplicatedUsing = OnRep_RespawnItem)
	bool bIsMeshVisiblity;

	UFUNCTION()
	void OnRep_RespawnItem();

	UPROPERTY(EditDefaultsOnly, Category = "PickupActor")
	float RespawnCoolTime; // ������Ǵµ� �ɸ��� �ð�

	FTimerHandle TimerHandle_RespawnTimer;

	void Respawn();


public:

	void StopPowerupEffect();

	void StartPowerupEffect();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:

	virtual void OnActivated();

	virtual void OnExpired();

	class ABaseCharacter* Player;

};
