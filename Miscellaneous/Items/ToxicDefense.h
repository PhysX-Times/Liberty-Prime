// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "ToxicDefense.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UToxicDefense : public UItem
{
	GENERATED_BODY()
	
public:

	UToxicDefense();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
