// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "RatRogueController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API ARatRogueController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	ARatRogueController();

	virtual void Tick(float DeltaTime) override;
	
};
