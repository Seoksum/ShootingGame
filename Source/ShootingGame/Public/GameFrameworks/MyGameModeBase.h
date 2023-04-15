// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AMyGameModeBase();

	UPROPERTY()
	TArray<APlayerController*> PlayerControllerList;

	virtual void PostLogin(class APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting);

	void PawnKilled(APawn* PawnKilled);
	void EndGame(APawn* InstigatorPawn, bool bMissionSuccess);



};
