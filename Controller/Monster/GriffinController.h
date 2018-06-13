// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "GriffinController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AGriffinController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	AGriffinController();

	virtual void Tick(float DeltaTime) override;
	
};
