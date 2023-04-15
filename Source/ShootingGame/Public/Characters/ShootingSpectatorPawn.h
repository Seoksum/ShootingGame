// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "ShootingSpectatorPawn.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API AShootingSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()
	
public:

	AShootingSpectatorPawn();

	virtual void SetupPlayerInputComponent(UInputComponent* InInputComponent) override;

	void ViewNextPlayer();

	void ViewPrevPlayer();
};
