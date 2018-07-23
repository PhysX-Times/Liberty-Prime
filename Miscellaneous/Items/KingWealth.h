// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "KingWealth.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UKingWealth : public UItem
{
	GENERATED_BODY()
	
public:

	UKingWealth();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
