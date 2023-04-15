// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PowerupActor_AddAmmo.h"
#include "Items/Weapon.h"
#include "Characters/BaseCharacter.h"

APowerupActor_AddAmmo::APowerupActor_AddAmmo()
{
	AddCount = 10;
}

void APowerupActor_AddAmmo::OnActivated()
{
	Weapon = Player->GetCurrentWeapon();
	if (Weapon)
	{
		Weapon->AddAmmoCount(AddCount);
	}
}

void APowerupActor_AddAmmo::OnExpired()
{

}