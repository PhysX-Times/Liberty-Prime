// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveToTarget.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Controller/MainAIController.h"
#include "LibertyPrimeCharacter.h"

EBTNodeResult::Type UMoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMainAIController* AIConRef = Cast<AMainAIController>(OwnerComp.GetAIOwner());

	if (AIConRef)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(AIConRef->EnemyKeyID));

		if (IsTarget)
		{
			ALibertyPrimeCharacter* MyOwner = Cast<ALibertyPrimeCharacter>(AIConRef->GetPawn());
			
			if (MyOwner)
			{
				AIConRef->MoveToActor(IsTarget, IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius() + MyOwner->GetCapsuleComponent()->GetScaledCapsuleRadius() + MyOwner->MoveTo_Distance_Add);
				return EBTNodeResult::Succeeded;
			}
			else
			{
				return EBTNodeResult::Failed;
			}
		}
		else
		{
			return EBTNodeResult::Failed;
		}
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}


