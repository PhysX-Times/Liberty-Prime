// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "BattleCry.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UBattleCry : public UItem
{
	GENERATED_BODY()
	
public:

	UBattleCry();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
