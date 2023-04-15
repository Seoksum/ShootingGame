// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MenuWidget.h"
#include "UI/InputIPWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetSwitcher.h"
#include "GameFrameworks/ShootingGameInstance.h"

void UMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	FInputModeUIOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;

	WBP_InputIP = CreateWidget<UInputIPWidget>(GetWorld(), WBP_InputIPClass);
	WBP_InputIP->RemoveFromViewport();

	MyGameInstance = Cast<UShootingGameInstance>(GetGameInstance());

	Btn_CreateServer->OnClicked.AddDynamic(this, &UMenuWidget::CreateServer);
	Btn_JoinServer->OnClicked.AddDynamic(this, &UMenuWidget::JoinServer);

}

void UMenuWidget::CreateServer()
{
	MyGameInstance->Host();
	//MyGameInstance->LaunchLobby(3, true);
}

void UMenuWidget::JoinServer()
{
	this->RemoveFromParent();
	WBP_InputIP = CreateWidget<UInputIPWidget>(GetWorld(), WBP_InputIPClass);
	WBP_InputIP->AddToViewport();

}
