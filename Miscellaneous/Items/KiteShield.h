// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "KiteShield.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UKiteShield : public UItem
{
	GENERATED_BODY()
	
public:

	UKiteShield();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
