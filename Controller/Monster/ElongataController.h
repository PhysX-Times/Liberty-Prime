// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "ElongataController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AElongataController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	AElongataController();

	virtual void Tick(float DeltaTime) override;
	
};
