// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "VenomBlade.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UVenomBlade : public UItem
{
	GENERATED_BODY()
	
public:

	UVenomBlade();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
