// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "DragonRexController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API ADragonRexController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	ADragonRexController();

	virtual void Tick(float DeltaTime) override;
	
};
