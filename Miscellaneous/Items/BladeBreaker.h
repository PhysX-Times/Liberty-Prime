// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "BladeBreaker.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UBladeBreaker : public UItem
{
	GENERATED_BODY()
	
public:

	UBladeBreaker();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
