// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "RockMonsterController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API ARockMonsterController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	ARockMonsterController();

	virtual void Tick(float DeltaTime) override;
	
};
