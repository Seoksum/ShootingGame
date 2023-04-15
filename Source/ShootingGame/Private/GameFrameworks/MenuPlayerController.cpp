// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameworks/MenuPlayerController.h"
#include "UI/MenuWidget.h"
#include "Kismet/GameplayStatics.h"


AMenuPlayerController::AMenuPlayerController()
{

}

void AMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (MenuClass)
	{
		MenuWidget = CreateWidget<UMenuWidget>(GetWorld(), MenuClass);
		MenuWidget->AddToViewport();
	}

	SetShowMouseCursor(true);
	SetInputMode(FInputModeUIOnly());

}