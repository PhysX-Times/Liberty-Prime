// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KnightColor.generated.h"

/**
*
*/
UCLASS()
class LIBERTYPRIME_API UKnightColor : public UObject
{
	GENERATED_BODY()

public:

	FLinearColor ArmorColor;
	TArray<FLinearColor> ClothColors;
};
