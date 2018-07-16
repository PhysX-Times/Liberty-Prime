// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "LifeDevourer.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API ULifeDevourer : public UItem
{
	GENERATED_BODY()
	
public:

	ULifeDevourer();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
