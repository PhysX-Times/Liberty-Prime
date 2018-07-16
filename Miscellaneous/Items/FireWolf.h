// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "FireWolf.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UFireWolf : public UItem
{
	GENERATED_BODY()
	
public:

	UFireWolf();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
