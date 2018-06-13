// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "WyvernController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AWyvernController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	AWyvernController();

	virtual void Tick(float DeltaTime) override;
	
};
