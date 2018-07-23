// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "LifeDestruction.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API ULifeDestruction : public UItem
{
	GENERATED_BODY()
	
public:

	ULifeDestruction();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
