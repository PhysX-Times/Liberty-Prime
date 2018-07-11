// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/Item.h"
#include "FireShell.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API UFireShell : public UItem
{
	GENERATED_BODY()
	
public:

	UFireShell();

	void Apply_Effect() override;
	void Reverse_Effect() override;
	
};
