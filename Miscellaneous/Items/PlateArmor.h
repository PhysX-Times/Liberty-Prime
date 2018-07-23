// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "PlateArmor.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UPlateArmor : public UItem
{
	GENERATED_BODY()
	
public:

	UPlateArmor();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
