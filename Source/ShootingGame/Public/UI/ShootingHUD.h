// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShootingHUD.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EHUDState : uint8
{
	Playing,
	Spectating,
	MatchEnd,
};

UCLASS()
class SHOOTINGGAME_API AShootingHUD : public AHUD
{
	GENERATED_BODY()

public:
	AShootingHUD();

	virtual void BeginPlay() override;

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;


private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairText;

	void DrawCrosshair();
	

public:

	UPROPERTY()
	UUserWidget* InGame;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = true))
	TSubclassOf<UUserWidget> InGameHUDClass;

	UPROPERTY()
	UUserWidget* Spectating;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = true))
	TSubclassOf<UUserWidget> SpectatingHUDClass;

	UPROPERTY()
	UUserWidget* MatchEnd;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = true))
	TSubclassOf<UUserWidget> MatchEndHUDClass;

	EHUDState CurrentState;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	EHUDState GetCurrentState() const;

	void HUDStateChanged(EHUDState NewState);

	void RemoveAllWidgets();

};
