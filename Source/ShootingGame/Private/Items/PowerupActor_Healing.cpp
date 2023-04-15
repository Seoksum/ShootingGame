// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PowerupActor_Healing.h"
#include "Kismet/GameplayStatics.h"
#include "Components/HealthComponent.h"
#include "Characters/BaseCharacter.h"


APowerupActor_Healing::APowerupActor_Healing()
{
	EffectDuration = 1.f;
	RespawnCoolTime = 15.f;
	HealingAmount = 15;
}

void APowerupActor_Healing::OnActivated()
{
	if (Player)
	{
		Player->HealPlayerHealth(HealingAmount);
	}
}

void APowerupActor_Healing::OnExpired()
{
	
}
