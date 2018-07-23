// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "VirtueJustice.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UVirtueJustice : public UItem
{
	GENERATED_BODY()
	
public:

	UVirtueJustice();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
