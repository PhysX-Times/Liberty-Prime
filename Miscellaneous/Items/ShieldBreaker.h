// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "ShieldBreaker.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UShieldBreaker : public UItem
{
	GENERATED_BODY()
	
public:

	UShieldBreaker();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
