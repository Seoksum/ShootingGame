// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoCountWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API UAmmoCountWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;


public:

	void SetCurrentAmmoCount(int32 AmmoCount);
	void SetTotalAmmoCount(int32 AmmoCount);

	int32 AmmoInClipCount;
	int32 TotalAmmoCount;


private:
	class UButton* Button;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentAmmo;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TotalAmmo;

	UPROPERTY()
	class ABaseCharacter* Player;

	UPROPERTY()
	class AWeapon* Weapon;

};
