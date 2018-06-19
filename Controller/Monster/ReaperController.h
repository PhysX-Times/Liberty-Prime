// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "ReaperController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AReaperController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	AReaperController();

	int random_skill;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
		
};
