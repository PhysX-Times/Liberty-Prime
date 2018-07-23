// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "CursePenance.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UCursePenance : public UItem
{
	GENERATED_BODY()
	
public:

	UCursePenance();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
