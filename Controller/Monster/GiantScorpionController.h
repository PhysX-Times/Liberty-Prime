// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "GiantScorpionController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AGiantScorpionController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	AGiantScorpionController();

	virtual void Tick(float DeltaTime) override;
	
	
};
