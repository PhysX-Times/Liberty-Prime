// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "WerewolfController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AWerewolfController : public AMonsterController
{
	GENERATED_BODY()
	
	
public:

	AWerewolfController();

	virtual void Tick(float DeltaTime) override;
	
};
