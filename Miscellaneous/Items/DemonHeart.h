// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "DemonHeart.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UDemonHeart : public UItem
{
	GENERATED_BODY()
	
public:

	UDemonHeart();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
