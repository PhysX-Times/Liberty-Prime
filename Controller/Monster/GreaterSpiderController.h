// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "GreaterSpiderController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AGreaterSpiderController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	AGreaterSpiderController();

	virtual void Tick(float DeltaTime) override;
		
};
