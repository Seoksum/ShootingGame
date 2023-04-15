// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ShootingPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "GameFrameworks/ShootingGameState.h"
#include "GameFramework/PlayerState.h"
#include "UI/ShootingHUD.h"
#include "GameFrameworks/MyGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


AShootingPlayerController::AShootingPlayerController()
{
	bRespawnImmediately = false;
}

void AShootingPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(false);
	SetInputMode(FInputModeUIOnly());
	SetPlayerPlay();
}

void AShootingPlayerController::OnReadyToStart()
{
	SetInputMode(FInputModeGameOnly());
}

void AShootingPlayerController::UnFreeze()
{
	Super::UnFreeze();

	SetPlayerSpectate();
	AShootingGameState* GS = Cast<AShootingGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		GS->RemovePlayerState(PlayerState);
	}
}

void AShootingPlayerController::SetPlayerPlay()
{
	if (!HasAuthority())
	{
		return;
	}

	PlayerState->SetIsSpectator(false);
	ChangeState(NAME_Playing);
	bPlayerIsWaiting = false;

	// 클라이언트에게 상태 업데이트
	ClientGotoState(NAME_Playing);
	ClientHUDStateChanged(EHUDState::Playing);
}

void AShootingPlayerController::SetPlayerSpectate()
{
	PlayerState->SetIsSpectator(true);
	ChangeState(NAME_Spectating);
	bPlayerIsWaiting = true;
	ClientGotoState(NAME_Spectating); //클라이언트에게 상태 업데이트
	ClientHUDStateChanged(EHUDState::Spectating);
	ViewAPlayer(1);//PlayerArray에서 다음 활성 플레이어
}

void AShootingPlayerController::ClientHUDStateChanged_Implementation(EHUDState NewState)
{
	AShootingHUD* HUD = GetHUD<AShootingHUD>();
	if (HUD)
	{
		HUD->HUDStateChanged(NewState);
	}
}