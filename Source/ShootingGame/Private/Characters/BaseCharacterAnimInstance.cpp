// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacterAnimInstance.h"
#include "Characters/BaseCharacter.h"
#include "GameFramework/PawnMovementComponent.h"


UBaseCharacterAnimInstance::UBaseCharacterAnimInstance()
{

}

void UBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		auto PlayerCharacter = Cast<ABaseCharacter>(Pawn);
		if (PlayerCharacter)
		{
			Speed = PlayerCharacter->GetVelocity().Size();
			Direction = CalculateDirection(PlayerCharacter->GetVelocity(), PlayerCharacter->GetActorRotation());

			IsFalling = PlayerCharacter->GetMovementComponent()->IsFalling();
			IsCrouched = PlayerCharacter->bIsCrouched;
			IsDeath = PlayerCharacter->bIsDeath;
			IsSprinting = PlayerCharacter->bIsSprinting;
			IsTargeting = PlayerCharacter->bIsTargeting;
		}
	}
}


