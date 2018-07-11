// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "PoisonDefense.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UPoisonDefense : public UItem
{
	GENERATED_BODY()
	
public:

	UPoisonDefense();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
