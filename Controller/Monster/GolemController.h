// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "GolemController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AGolemController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	AGolemController();

	virtual void Tick(float DeltaTime) override;
	
};
