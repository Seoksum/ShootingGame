// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Powerup_SuperSpeed.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Characters/BaseCharacter.h"


APowerup_SuperSpeed::APowerup_SuperSpeed()
{
	EffectDuration = 5.f;
	RespawnCoolTime = 10.f;

}

void APowerup_SuperSpeed::OnActivated()
{
	if (Player)
	{
		UCharacterMovementComponent* MovementComp = Player->GetCharacterMovement();
		MovementComp->MaxWalkSpeed = MovementComp->GetMaxSpeed() * 1.8f;
	}
}

void APowerup_SuperSpeed::OnExpired()
{
	if (Player)
	{
		UCharacterMovementComponent* MovementComp = Player->GetCharacterMovement();
		MovementComp->MaxWalkSpeed = MovementComp->GetMaxSpeed() / 1.8f;
	}

}
