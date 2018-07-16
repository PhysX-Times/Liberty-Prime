// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "LoneWolf.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API ULoneWolf : public UItem
{
	GENERATED_BODY()
	
public:

	ULoneWolf();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
