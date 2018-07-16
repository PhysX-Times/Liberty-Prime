// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "MysticCreature.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UMysticCreature : public UItem
{
	GENERATED_BODY()
	
public:

	UMysticCreature();

	void Apply_Effect() override;
	void Reverse_Effect() override;

};
