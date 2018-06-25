// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "BoneDragon_TwinController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API ABoneDragon_TwinController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	ABoneDragon_TwinController();

	virtual void Tick(float DeltaTime) override;
	
};
