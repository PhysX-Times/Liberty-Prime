// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "GiantWormController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AGiantWormController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	AGiantWormController();

	virtual void Tick(float DeltaTime) override;
	
};
