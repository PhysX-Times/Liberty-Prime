// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "EnvySin.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UEnvySin : public UItem
{
	GENERATED_BODY()
	
public:

	UEnvySin();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
