// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AmmoCountWidget.h"
#include "Characters/BaseCharacter.h"
#include "Items/Weapon.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
//#include "..\..\Public\UI\AmmoCountWidget.h"

void UAmmoCountWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Player = Cast<ABaseCharacter>(GetOwningPlayerPawn());
	if (Player)
	{
		Weapon = Player->GetCurrentWeapon();
		if (Weapon)
		{
			Weapon->OnAmmoInClipChanged.AddUObject(this, &UAmmoCountWidget::SetCurrentAmmoCount);
			Weapon->OnTotalAmmoChanged.AddUObject(this, &UAmmoCountWidget::SetTotalAmmoCount);
		}
	}
}


void UAmmoCountWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	

}

void UAmmoCountWidget::SetCurrentAmmoCount(int32 AmmoCount)
{
	const FString Count = FString::Printf(TEXT("%d"), AmmoCount);
	CurrentAmmo->SetText(FText::FromString(Count));
}

void UAmmoCountWidget::SetTotalAmmoCount(int32 AmmoCount)
{
	const FString Count = FString::Printf(TEXT("%d"), AmmoCount);
	TotalAmmo->SetText(FText::FromString(Count));
}

