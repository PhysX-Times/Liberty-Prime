// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "NightRaven.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UNightRaven : public UItem
{
	GENERATED_BODY()
	
public:

	UNightRaven();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
