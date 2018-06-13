// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

class APlayerChar;

UCLASS()
class LIBERTYPRIME_API UItem : public UObject
{
	GENERATED_BODY()
	
public:

	UItem();

	APlayerChar* MyOwner;

	
	UPROPERTY(BlueprintReadWrite, Category = "Detail")
	int Item_Tier;
	UPROPERTY(BlueprintReadWrite, Category = "Detail")
	UTexture2D* Item_Icon;
	UPROPERTY(BlueprintReadWrite, Category = "Detail")
	FString Item_Name;
	UPROPERTY(BlueprintReadWrite, Category = "Detail", meta = (MultiLine = true))
	FString Item_Description;

	virtual void Apply_Effect();
	virtual void Reverse_Effect();
	
};
