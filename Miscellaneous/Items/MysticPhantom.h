// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "MysticPhantom.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UMysticPhantom : public UItem
{
	GENERATED_BODY()
	
public:

	UMysticPhantom();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
