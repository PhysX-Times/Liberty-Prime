// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "GorosaurusController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AGorosaurusController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	AGorosaurusController();

	virtual void Tick(float DeltaTime) override;
	
};
