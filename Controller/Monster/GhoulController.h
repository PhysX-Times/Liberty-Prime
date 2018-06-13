// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "GhoulController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AGhoulController : public AMonsterController
{
	GENERATED_BODY()
	
public:
	
	AGhoulController();

	virtual void Tick(float DeltaTime) override;
};
