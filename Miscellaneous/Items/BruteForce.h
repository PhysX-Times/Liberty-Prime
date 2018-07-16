// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "BruteForce.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UBruteForce : public UItem
{
	GENERATED_BODY()
	
public:

	UBruteForce();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
