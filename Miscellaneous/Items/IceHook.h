// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "IceHook.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UIceHook : public UItem
{
	GENERATED_BODY()
	
public:

	UIceHook();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
