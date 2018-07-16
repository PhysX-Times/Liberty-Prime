// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "ObsidianDemon.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UObsidianDemon : public UItem
{
	GENERATED_BODY()
	
public:

	UObsidianDemon();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
