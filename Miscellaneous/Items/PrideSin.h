// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "PrideSin.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UPrideSin : public UItem
{
	GENERATED_BODY()
	
public:

	UPrideSin();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
