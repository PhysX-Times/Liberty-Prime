// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "JackTrade.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UJackTrade : public UItem
{
	GENERATED_BODY()
	
public:

	UJackTrade();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
