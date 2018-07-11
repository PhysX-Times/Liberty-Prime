// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "VenomSerpent.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UVenomSerpent : public UItem
{
	GENERATED_BODY()
	
public:

	UVenomSerpent();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
