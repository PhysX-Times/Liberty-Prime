// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "FogHill.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UFogHill : public UItem
{
	GENERATED_BODY()
	
public:

	UFogHill();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
