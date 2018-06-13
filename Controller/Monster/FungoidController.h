// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "FungoidController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AFungoidController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	AFungoidController();

	virtual void Tick(float DeltaTime) override;
	
};
