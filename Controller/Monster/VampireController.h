// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "VampireController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AVampireController : public AMonsterController
{
	GENERATED_BODY()
		
public:

	AVampireController();

	virtual void Tick(float DeltaTime) override;
	
};
