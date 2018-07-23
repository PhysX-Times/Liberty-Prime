// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "LustSin.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API ULustSin : public UItem
{
	GENERATED_BODY()
	
public:

	ULustSin();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
