// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "PoisonousDemonTerror.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UPoisonousDemonTerror : public UItem
{
	GENERATED_BODY()
	
public:

	UPoisonousDemonTerror();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
