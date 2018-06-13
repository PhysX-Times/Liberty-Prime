// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "PlantMonsterController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API APlantMonsterController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	APlantMonsterController();

	virtual void Tick(float DeltaTime) override;
	
};
