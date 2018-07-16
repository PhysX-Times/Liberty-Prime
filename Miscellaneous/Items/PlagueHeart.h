// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "PlagueHeart.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UPlagueHeart : public UItem
{
	GENERATED_BODY()
	
public:

	UPlagueHeart();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
