// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "EyeMonsterController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AEyeMonsterController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	AEyeMonsterController();

	virtual void Tick(float DeltaTime) override;
	
};
