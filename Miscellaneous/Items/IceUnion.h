// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "IceUnion.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UIceUnion : public UItem
{
	GENERATED_BODY()
	
public:

	UIceUnion();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
