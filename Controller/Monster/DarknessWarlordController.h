// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "DarknessWarlordController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API ADarknessWarlordController : public AMonsterController
{
	GENERATED_BODY()

public:
	
	ADarknessWarlordController();

	virtual void Tick(float DeltaTime) override;
	
	
};
