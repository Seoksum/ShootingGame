// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameworks/ShootingGameState.h"
#include "Characters/ShootingPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/ShootingPlayerState.h"
#include "Engine/World.h"
#include "EngineUtils.h"

AShootingGameState::AShootingGameState()
{
	MaxNumOfPlayers = 3;
}

void AShootingGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void AShootingGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	AShootingPlayerState* PS = Cast<AShootingPlayerState>(PlayerState);
	if (PS)
	{
		PlayerArray.AddUnique(PS);
	}

	if (PlayerArray.Num() == MaxNumOfPlayers)
	{
		OnReadyToStart();
	}
}

void AShootingGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);

	PlayerArray.Remove(PlayerState);

	if (PlayerArray.Num() == 1)
	{
		EndGame();
	}
}

void AShootingGameState::OnReadyToStart()
{
	for (AShootingPlayerController* Controller : TActorRange<AShootingPlayerController>(GetWorld()))
	{
		Controller->SetInputMode(FInputModeGameOnly());
	}
}

void AShootingGameState::EndGame()
{
	for (AShootingPlayerController* Controller : TActorRange<AShootingPlayerController>(GetWorld()))
	{
		Controller->ClientHUDStateChanged(EHUDState::MatchEnd);
		DisableInput(Controller);
		
	}
}


