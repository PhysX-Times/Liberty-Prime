// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "WeresquidController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AWeresquidController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	AWeresquidController();

	int skill_rand;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
