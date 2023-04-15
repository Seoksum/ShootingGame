// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InputIPWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "GameFrameworks/ShootingGameInstance.h"

void UInputIPWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	MyGameInstance = Cast<UShootingGameInstance>(GetGameInstance());

	Btn_Join->OnClicked.AddDynamic(this, &UInputIPWidget::JoinServer);
	
}

void UInputIPWidget::JoinServer()
{
	MyGameInstance->Join(IPAddressField->GetText().ToString());
	RemoveFromParent();
}