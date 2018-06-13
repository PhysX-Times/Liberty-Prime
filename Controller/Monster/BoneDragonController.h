// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "BoneDragonController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API ABoneDragonController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	ABoneDragonController();

	virtual void Tick(float DeltaTime) override;
	
};
