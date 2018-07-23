// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "SlothSin.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API USlothSin : public UItem
{
	GENERATED_BODY()
	
public:

	USlothSin();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
