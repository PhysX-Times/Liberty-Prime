// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "TrollController.generated.h"

/**
*
*/
UCLASS()
class LIBERTYPRIME_API ATrollController : public AMonsterController
{
	GENERATED_BODY()

public:

	ATrollController();

	virtual void Tick(float DeltaTime) override;
};
