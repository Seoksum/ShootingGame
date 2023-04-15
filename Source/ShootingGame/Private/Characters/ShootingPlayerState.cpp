// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ShootingPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFrameworks/ShootingGameState.h"

AShootingPlayerState::AShootingPlayerState()
{
	AShootingGameState* GS = Cast<AShootingGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		GS->AddPlayerState(this);
	}
}
