// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PowerupActor.h"
#include "PowerupActor_Healing.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API APowerupActor_Healing : public APowerupActor
{
	GENERATED_BODY()
	
public:
	APowerupActor_Healing();


protected:

	void OnActivated() override;

	void OnExpired() override;


private:

	int32 HealingAmount;
};
