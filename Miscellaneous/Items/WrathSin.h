// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "WrathSin.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UWrathSin : public UItem
{
	GENERATED_BODY()
	
public:

	UWrathSin();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
