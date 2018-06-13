// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "MainAgroCheck.generated.h"

/**
*
*/
UCLASS()
class LIBERTYPRIME_API UMainAgroCheck : public UBTService
{
	GENERATED_BODY()

public:

	UMainAgroCheck();

	bool bBuildUp;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
