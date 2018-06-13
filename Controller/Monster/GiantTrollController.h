// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "GiantTrollController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AGiantTrollController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	AGiantTrollController();

	virtual void Tick(float DeltaTime) override;
	
};
