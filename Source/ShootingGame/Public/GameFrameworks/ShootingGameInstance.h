// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ShootingGameInstance.generated.h"

/**
 *
 */


UCLASS()
class SHOOTINGGAME_API UShootingGameInstance : public UGameInstance
{
	GENERATED_BODY()


public:

	UShootingGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;

	UFUNCTION(Exec)
	void Host(); //ȣ��Ʈ

	UFUNCTION(Exec) //���� + ���Ӿ�����
	void Join(const FString& Address);


};
