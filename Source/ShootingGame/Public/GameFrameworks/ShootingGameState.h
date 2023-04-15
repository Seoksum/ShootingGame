// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ShootingGameState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnReadyToStartPlayers);
/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API AShootingGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	AShootingGameState();

	virtual void PostInitializeComponents() override;

	virtual void AddPlayerState(APlayerState* PlayerState);
	virtual void RemovePlayerState(APlayerState* PlayerState);

	void OnReadyToStart();

	void EndGame();

private:

	int32 MaxNumOfPlayers;

};
