// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ShootingHUD.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


AShootingHUD::AShootingHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("Texture2D'/Game/Images/FirstPersonCrosshair.FirstPersonCrosshair'"));
	CrosshairText = CrosshairTexObj.Object;

	static ConstructorHelpers::FClassFinder<UUserWidget> LayoutClass(TEXT("WidgetBlueprint'/Game/UI/InGame/WBP_HUDLayout.WBP_HUDLayout_C'"));
	if (LayoutClass.Succeeded())
	{
		LayoutHUDClass = LayoutClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> SpectatingClass(TEXT("WidgetBlueprint'/Game/UI/InGame/WBP_Spectator.WBP_Spectator_C'"));
	if (SpectatingClass.Succeeded())
	{
		SpectatingHUDClass = SpectatingClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> MatchEndClass(TEXT("WidgetBlueprint'/Game/UI/InGame/WBP_MatchEnd.WBP_MatchEnd_C'"));
	if (MatchEndClass.Succeeded())
	{
		MatchEndHUDClass = MatchEndClass.Class;
	}
}

void AShootingHUD::BeginPlay()
{
	Spectating = CreateWidget(GetWorld(), SpectatingHUDClass);
	MatchEnd = CreateWidget(GetWorld(), MatchEndHUDClass);
	Layout = CreateWidget(GetWorld(), LayoutHUDClass);
	Layout->AddToViewport();
	CurrentWidget = Layout;
}

void AShootingHUD::DrawHUD()
{
	Super::DrawHUD();

	if (CrosshairText)
	{
		DrawCrosshair();
	}
}

void AShootingHUD::DrawCrosshair()
{
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition((Center.X - 20), (Center.Y));

	// draw the crosshair
	FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairText->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
}

EHUDState AShootingHUD::GetCurrentState() const
{
	return CurrentState;
}

void AShootingHUD::HUDStateChanged(EHUDState NewState)
{
	RemoveAllWidgets();
	UUserWidget* TempWidget = nullptr;
	switch (NewState)
	{
	case EHUDState::Playing :
		TempWidget = Layout;
		break;
	case EHUDState::Spectating:
		TempWidget = Spectating;
		break;
	case EHUDState::MatchEnd:
		TempWidget = MatchEnd;
		break;
	}
	if (TempWidget)
	{
		TempWidget->AddToViewport();
	}
}

void AShootingHUD::OnStateChanged_Implementation(EHUDState NewState)
{
	CurrentState = NewState;
}

void AShootingHUD::RemoveAllWidgets()
{
	TArray<UUserWidget*> RemoveWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), RemoveWidgets,UUserWidget::StaticClass());

	for (UUserWidget* Widget : RemoveWidgets)
	{
		Widget->RemoveFromParent();
	}

}

