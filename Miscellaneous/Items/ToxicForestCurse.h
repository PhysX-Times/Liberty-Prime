// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "ToxicForestCurse.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UToxicForestCurse : public UItem
{
	GENERATED_BODY()
	
public:

	UToxicForestCurse();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
