// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PowerupActor.h"
#include "Powerup_SuperSpeed.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API APowerup_SuperSpeed : public APowerupActor
{
	GENERATED_BODY()

public:

	APowerup_SuperSpeed();


protected:

	void OnActivated() override;


	void OnExpired() override;

};
