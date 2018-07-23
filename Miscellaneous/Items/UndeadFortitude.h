// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "UndeadFortitude.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UUndeadFortitude : public UItem
{
	GENERATED_BODY()
	
public:

	UUndeadFortitude();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
	
};
