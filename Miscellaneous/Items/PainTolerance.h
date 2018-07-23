// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "PainTolerance.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UPainTolerance : public UItem
{
	GENERATED_BODY()
	
public:

	UPainTolerance();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
