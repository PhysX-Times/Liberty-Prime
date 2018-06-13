// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "Inferno.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UInferno : public UItem
{
	GENERATED_BODY()

public:

	UInferno();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
