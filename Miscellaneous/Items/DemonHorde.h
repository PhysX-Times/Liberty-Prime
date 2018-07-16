// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "DemonHorde.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UDemonHorde : public UItem
{
	GENERATED_BODY()
	
public:

	UDemonHorde();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
