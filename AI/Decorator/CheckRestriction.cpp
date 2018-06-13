// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckRestriction.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "LibertyPrimeCharacter.h"
#include "Controller/MainAIController.h"

bool UCheckRestriction::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AMainAIController* AIConRef = Cast<AMainAIController>(OwnerComp.GetAIOwner());
	bool result = false;

	if (AIConRef)
	{
		ALibertyPrimeCharacter* MyPawn = Cast<ALibertyPrimeCharacter>(AIConRef->GetPawn());

		if (MyPawn)
		{
			if (MyPawn->CheckRestriction())
			{
				result = true;
			}
		}
	}

	return result;
}