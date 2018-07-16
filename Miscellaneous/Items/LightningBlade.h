// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "LightningBlade.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API ULightningBlade : public UItem
{
	GENERATED_BODY()
	
public:

	ULightningBlade();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
