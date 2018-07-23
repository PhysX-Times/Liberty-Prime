// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "VirtueCourage.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UVirtueCourage : public UItem
{
	GENERATED_BODY()
	
public:

	UVirtueCourage();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
