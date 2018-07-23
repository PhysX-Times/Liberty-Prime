// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "BloodLust.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UBloodLust : public UItem
{
	GENERATED_BODY()
	
public:

	UBloodLust();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
