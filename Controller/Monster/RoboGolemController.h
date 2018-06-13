// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "RoboGolemController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API ARoboGolemController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	ARoboGolemController();

	virtual void Tick(float DeltaTime) override;
	
};
