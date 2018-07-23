// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "VirtueFortitude.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UVirtueFortitude : public UItem
{
	GENERATED_BODY()
	
public:

	UVirtueFortitude();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
