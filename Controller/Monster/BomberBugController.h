// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "BomberBugController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API ABomberBugController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	ABomberBugController();

	virtual void Tick(float DeltaTime) override;
	
};
