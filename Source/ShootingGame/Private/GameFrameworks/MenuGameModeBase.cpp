// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameworks/MenuGameModeBase.h"
#include "GameFrameworks/MenuPlayerController.h"

AMenuGameModeBase::AMenuGameModeBase()
{
	PlayerControllerClass = AMenuPlayerController::StaticClass();
}