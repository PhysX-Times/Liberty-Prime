// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "MonsterBossController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AMonsterBossController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	AMonsterBossController();

	virtual void Tick(float DeltaTime) override;
	
};
