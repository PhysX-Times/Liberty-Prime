// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "DemonRider.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UDemonRider : public UItem
{
	GENERATED_BODY()
	
public:

	UDemonRider();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
