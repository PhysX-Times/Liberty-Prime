// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "GreedSin.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UGreedSin : public UItem
{
	GENERATED_BODY()
	
public:

	UGreedSin();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
