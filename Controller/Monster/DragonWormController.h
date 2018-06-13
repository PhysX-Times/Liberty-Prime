// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "DragonWormController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API ADragonWormController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	ADragonWormController();

	virtual void Tick(float DeltaTime) override;
	
};
