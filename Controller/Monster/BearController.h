// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "BearController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API ABearController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	ABearController();

	virtual void Tick(float DeltaTime) override;
	
};
