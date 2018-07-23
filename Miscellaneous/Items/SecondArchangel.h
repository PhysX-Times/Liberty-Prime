// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "SecondArchangel.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API USecondArchangel : public UItem
{
	GENERATED_BODY()
	
public:

	USecondArchangel();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
