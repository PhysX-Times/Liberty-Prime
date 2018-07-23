// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "VenomFlesh.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UVenomFlesh : public UItem
{
	GENERATED_BODY()
	
public:

	UVenomFlesh();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
