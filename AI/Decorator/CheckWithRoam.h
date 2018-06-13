// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "CheckWithRoam.generated.h"

/**
*
*/
UCLASS()
class LIBERTYPRIME_API UCheckWithRoam : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
