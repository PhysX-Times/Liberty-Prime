// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "TreeEntController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API ATreeEntController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	ATreeEntController();

	virtual void Tick(float DeltaTime) override;
	
};
