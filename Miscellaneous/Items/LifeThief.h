// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "LifeThief.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API ULifeThief : public UItem
{
	GENERATED_BODY()
	
public:

	ULifeThief();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
