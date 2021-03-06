// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "SkeletonSorcererController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API ASkeletonSorcererController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	ASkeletonSorcererController();

	int rand_skill;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
