// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "ChimeraController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AChimeraController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	AChimeraController();

	virtual void Tick(float DeltaTime) override;
		
};
