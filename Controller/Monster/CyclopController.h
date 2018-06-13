// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "CyclopController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API ACyclopController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	ACyclopController();

	virtual void Tick(float DeltaTime) override;
	
};
