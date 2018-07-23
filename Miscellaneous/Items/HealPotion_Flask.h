// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "HealPotion_Flask.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UHealPotion_Flask : public UItem
{
	GENERATED_BODY()
	
public:

	UHealPotion_Flask();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
