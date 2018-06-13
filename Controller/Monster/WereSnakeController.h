// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/MainAIController.h"
#include "WereSnakeController.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AWereSnakeController : public AMainAIController
{
	GENERATED_BODY()
	
public:

	AWereSnakeController();

	virtual void Tick(float DeltaTime) override;
	
	
};
