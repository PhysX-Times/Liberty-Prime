// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "MotherNature.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UMotherNature : public UItem
{
	GENERATED_BODY()
	
public:

	UMotherNature();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
