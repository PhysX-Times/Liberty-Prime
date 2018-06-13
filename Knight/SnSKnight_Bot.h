// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Knight/SnSKnight.h"
#include "SnSKnight_Bot.generated.h"

/**
*
*/
UCLASS()
class LIBERTYPRIME_API ASnSKnight_Bot : public ASnSKnight
{
	GENERATED_BODY()

public:

	ASnSKnight_Bot();

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<class ASword> SwordClass;

	virtual void BeginPlay() override;
};
