// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathIndicator.generated.h"

UCLASS()
class LIBERTYPRIME_API APathIndicator : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere, Category = "Variables")
		bool CanWait;

	UPROPERTY(EditAnywhere, Category = "Variables")
		APathIndicator* NextTarget;
	
};
