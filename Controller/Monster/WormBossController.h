// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "WormBossController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AWormBossController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	AWormBossController();

	virtual void Tick(float DeltaTime) override;
	
};
