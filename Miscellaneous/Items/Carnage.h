// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "Carnage.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UCarnage : public UItem
{
	GENERATED_BODY()
	
public:

	UCarnage();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
