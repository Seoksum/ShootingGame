// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HpWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API UHpWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_HpBar;

	TWeakObjectPtr<class UHealthComponent> CurrentHealthComp;


};
