// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "GateGuardian.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UGateGuardian : public UItem
{
	GENERATED_BODY()
	
public:

	UGateGuardian();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
