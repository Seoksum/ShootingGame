// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HpWidget.h"
#include "Components/HealthComponent.h"
#include "Components/ProgressBar.h"
#include "Characters/BaseCharacter.h"


void UHpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetOwningPlayerPawn());
	if (BaseCharacter)
	{
		CurrentHealthComp = BaseCharacter->HealthComp;
	}
}


void UHpWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (CurrentHealthComp.IsValid())
	{
		PB_HpBar->SetPercent(CurrentHealthComp->GetHpRatio());
	}

}


