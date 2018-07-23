// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "FireNation.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UFireNation : public UItem
{
	GENERATED_BODY()
	
public:

	UFireNation();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
