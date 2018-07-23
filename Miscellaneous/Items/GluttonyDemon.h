// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "GluttonyDemon.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UGluttonyDemon : public UItem
{
	GENERATED_BODY()
	
public:

	UGluttonyDemon();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
