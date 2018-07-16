// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "IceSeal.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UIceSeal : public UItem
{
	GENERATED_BODY()
	
public:

	UIceSeal();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
