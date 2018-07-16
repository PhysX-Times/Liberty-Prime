// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "FireSeal.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UFireSeal : public UItem
{
	GENERATED_BODY()
	
public:

	UFireSeal();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
