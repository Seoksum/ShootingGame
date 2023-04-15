// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameworks/MyGameModeBase.h"
#include "GameFramework/Controller.h"
#include "EngineUtils.h"
#include "GameFrameworks/ShootingGameState.h"
#include "UI/ShootingHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/ShootingPlayerController.h"


AMyGameModeBase::AMyGameModeBase()
{
	GameStateClass = AShootingGameState::StaticClass();

	
}

void AMyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AShootingGameState* GS = Cast<AShootingGameState>(UGameplayStatics::GetGameState(GetWorld()));
	UE_LOG(LogTemp, Log, TEXT("MyLogin : %d"), GS->PlayerArray.Num());

}

void AMyGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

}


