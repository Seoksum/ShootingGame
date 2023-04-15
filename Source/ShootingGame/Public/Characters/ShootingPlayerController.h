
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/ShootingHUD.h"
#include "ShootingPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API AShootingPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AShootingPlayerController();

	virtual void BeginPlay() override;

	void OnReadyToStart();


	/* 사망 후 부활 또는 관전 시작 */
	virtual void UnFreeze() override;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
		void SetPlayerPlay();

	UFUNCTION(BlueprintCallable)
		void SetPlayerSpectate();

	UFUNCTION(Reliable, Client)
	void ClientHUDStateChanged(EHUDState NewState);

	void ClientHUDStateChanged_Implementation(EHUDState NewState);

private:
	
	/* 사망 시 부활하거나 관전을 시작하도록 플래그 지정 */
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	bool bRespawnImmediately;

};
