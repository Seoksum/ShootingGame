// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ShootingSpectatorPawn.h"

AShootingSpectatorPawn::AShootingSpectatorPawn()
{
	bAddDefaultMovementBindings = true;
}

void AShootingSpectatorPawn::SetupPlayerInputComponent(UInputComponent* InInputComponent)
{
    Super::SetupPlayerInputComponent(InInputComponent);

    InInputComponent->BindAction("ViewNext", IE_Pressed, this, &ThisClass::ViewNextPlayer);
    InInputComponent->BindAction("ViewPrev", IE_Pressed, this, &ThisClass::ViewPrevPlayer);
}

void AShootingSpectatorPawn::ViewNextPlayer()
{
    if (APlayerController* PC = GetController<APlayerController>())
    {
        PC->ServerViewNextPlayer();
    }
}

void AShootingSpectatorPawn::ViewPrevPlayer()
{
    if (APlayerController* PC = GetController<APlayerController>())
    {
        PC->ServerViewPrevPlayer();
    }
}