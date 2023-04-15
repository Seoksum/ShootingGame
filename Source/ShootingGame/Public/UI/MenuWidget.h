// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized();

	UFUNCTION()
		virtual void CreateServer();

	UFUNCTION()
		virtual void JoinServer();


public:

	UPROPERTY(meta = (BindWidget))
		class UButton* Btn_CreateServer;

	UPROPERTY(meta = (BindWidget))
		class UButton* Btn_JoinServer;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<class UInputIPWidget> WBP_InputIPClass;

	UPROPERTY()
		class UInputIPWidget* WBP_InputIP;

	UPROPERTY()
		class UShootingGameInstance* MyGameInstance;


};
