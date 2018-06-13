// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "SoulEaterController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API ASoulEaterController : public AMonsterController
{
	GENERATED_BODY()
	
	
public:

	ASoulEaterController();

	virtual void Tick(float DeltaTime) override;
	
};
