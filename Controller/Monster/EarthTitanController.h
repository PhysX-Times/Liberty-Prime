// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "EarthTitanController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AEarthTitanController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	AEarthTitanController();

	virtual void Tick(float DeltaTime) override;
	
};
