// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "HeartCreation.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UHeartCreation : public UItem
{
	GENERATED_BODY()
	
public:

	UHeartCreation();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
