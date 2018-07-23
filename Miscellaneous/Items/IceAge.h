// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "IceAge.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UIceAge : public UItem
{
	GENERATED_BODY()
	
public:

	UIceAge();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
