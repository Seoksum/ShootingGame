
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


	/* ��� �� ��Ȱ �Ǵ� ���� ���� */
	virtual void UnFreeze() override;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
		void SetPlayerPlay();

	UFUNCTION(BlueprintCallable)
		void SetPlayerSpectate();

	UFUNCTION(Reliable, Client)
	void ClientHUDStateChanged(EHUDState NewState);

	void ClientHUDStateChanged_Implementation(EHUDState NewState);

private:
	
	/* ��� �� ��Ȱ�ϰų� ������ �����ϵ��� �÷��� ���� */
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	bool bRespawnImmediately;

};
