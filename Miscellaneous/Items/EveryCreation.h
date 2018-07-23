// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "EveryCreation.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UEveryCreation : public UItem
{
	GENERATED_BODY()
	
public:

	UEveryCreation();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
