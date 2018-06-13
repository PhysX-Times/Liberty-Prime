// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Knight/KnightController.h"
#include "SnSKnightController.generated.h"

/**
*
*/
UCLASS()
class LIBERTYPRIME_API ASnSKnightController : public AKnightController
{
	GENERATED_BODY()

public:

	ASnSKnightController();

	bool IsRoaming;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
