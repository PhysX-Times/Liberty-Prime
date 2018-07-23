// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "MadKingSword.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UMadKingSword : public UItem
{
	GENERATED_BODY()
	
public:

	UMadKingSword();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
