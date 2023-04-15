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
	UWorld* World = GetWorld(); // ������ ���� ������ ������


	//�������� .  ������ �Է��� ����� ������ �̵���Ŵ, Ŭ���̾�Ʈ�� ���̰� ������ �������� Ŭ���̾�Ʈ����
	//�÷��̾� ��Ʈ�ѷ����� ClientTravel�� ȣ����.
	World->ServerTravel("/Game/Maps/BattleMap?listen");
}

void UShootingGameInstance::Join(const FString& Address)
{	
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

