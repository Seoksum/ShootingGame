// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PowerupActor.h"
#include "PowerupActor_AddAmmo.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API APowerupActor_AddAmmo : public APowerupActor
{
	GENERATED_BODY()

public:

	APowerupActor_AddAmmo();

	UPROPERTY(EditDefaultsOnly,Category="Ammo")
	int32 AddCount;

protected:

	void OnActivated() override;

	void OnExpired() override;


private:

	class AWeapon* Weapon;

	int32 AmmoCount;


};
