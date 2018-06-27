// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "MountainDragonController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AMountainDragonController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	float Distance;

	AMountainDragonController();

	virtual void Tick(float DeltaTime) override;
	
};
