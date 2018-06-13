// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "GargoyleController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AGargoyleController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	AGargoyleController();

	virtual void Tick(float DeltaTime) override;
	
};
