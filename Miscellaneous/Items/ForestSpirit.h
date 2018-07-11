// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "ForestSpirit.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UForestSpirit : public UItem
{
	GENERATED_BODY()
	
public:

	UForestSpirit();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
