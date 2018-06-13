// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "MasterGruntController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AMasterGruntController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	AMasterGruntController();

	virtual void Tick(float DeltaTime) override;
	
	
};
