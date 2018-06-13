// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckWait.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Controller/MainAIController.h"

bool UCheckWait::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AMainAIController* AIConRef = Cast<AMainAIController>(OwnerComp.GetAIOwner());
	bool result = false;

	if (AIConRef)
	{
		result = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Bool>(AIConRef->WaitKeyID);
	}

	return result;
}