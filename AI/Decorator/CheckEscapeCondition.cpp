// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckEscapeCondition.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Monster/MountainDragon.h"
#include "Controller/Monster/MountainDragonController.h"

bool UCheckEscapeCondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AMountainDragonController* AIConRef = Cast<AMountainDragonController>(OwnerComp.GetAIOwner());
	bool result = false;

	if (AIConRef)
	{
		AMountainDragon* MyPawn = Cast<AMountainDragon>(AIConRef->GetPawn());

		if (MyPawn)
		{
			if (!MyPawn->IsDead && AIConRef->Distance < MyPawn->AttackDistance - 75.0f)
			{
				result = true;
			}
		}
	}

	return result;
}