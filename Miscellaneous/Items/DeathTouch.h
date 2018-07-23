// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "DeathTouch.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UDeathTouch : public UItem
{
	GENERATED_BODY()
	
public:

	UDeathTouch();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
