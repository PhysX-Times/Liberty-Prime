// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "LibertyPrimePlayerController.generated.h"

USTRUCT(BlueprintType)
struct FIndicatorData
{
	GENERATED_BODY()

	FIndicatorData()
	{

	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IndicatorData")
	UUserWidget* InitWidget;
	UPROPERTY()
	FVector InitLocation;

	FIndicatorData(UUserWidget* InitWidget, FVector InitLocation)
	{
		this->InitWidget = InitWidget;
		this->InitLocation = InitLocation;
	}
};

UCLASS()
class ALibertyPrimePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALibertyPrimePlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG")
	TSubclassOf<UUserWidget> DMGIndicator_Class;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG")
	TSubclassOf<UUserWidget> HealthIndictor_Class;

	UPROPERTY(BlueprintReadWrite, Category = "UMG")
	TArray<FIndicatorData> DMGIndicatorDatas;

	void Add_Indicator(float Val, FLinearColor Color, FVector Location);

	UFUNCTION(BlueprintImplementableEvent, Category = "CppFunctions")
	void Indicator_Event(UUserWidget* TargetWidget, float Val, FLinearColor Color);
	void Indicator_Event_Implementation(UUserWidget* TargetWidget, float Val, FLinearColor Color);

protected:

	virtual void PlayerTick(float DeltaTime) override;
};


