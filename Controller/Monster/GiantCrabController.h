// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "GiantCrabController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AGiantCrabController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	AGiantCrabController();

	virtual void Tick(float DeltaTime) override;
	
	
};
