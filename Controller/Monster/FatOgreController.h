// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Monster/MonsterController.h"
#include "FatOgreController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AFatOgreController : public AMonsterController
{
	GENERATED_BODY()
	
public:

	AFatOgreController();

	virtual void Tick(float DeltaTime) override;
	
};
