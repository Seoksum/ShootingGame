// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameworks/ShootingGameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Blueprint/UserWidget.h"
#include "GameFrameworks\ShootingGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFrameworks/MyGameModeBase.h"
#include "Characters/ShootingPlayerController.h"


UShootingGameInstance::UShootingGameInstance(const FObjectInitializer& ObjectInitializer)
{

}

void UShootingGameInstance::Init()
{
	Super::Init();
}

void UShootingGameInstance::Host()
{
	UWorld* World = GetWorld(); // 레벨의 현재 레벨을 가져옴


	//서버전용 .  서버를 입력한 경로의 맵으로 이동시킴, 클라이언트랑 같이감 서버가 접속중인 클라이언트들의
	//플레이어 컨트롤러에서 ClientTravel을 호출함.
	World->ServerTravel("/Game/Maps/BattleMap?listen");
}

void UShootingGameInstance::Join(const FString& Address)
{	
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

