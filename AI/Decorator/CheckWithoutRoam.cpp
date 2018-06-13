// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckWithoutRoam.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Controller/MainAIController.h"
#include "LibertyPrimeCharacter.h"
#include "Kismet/KismetMathLibrary.h"

bool UCheckWithoutRoam::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AMainAIController* AIConRef = Cast<AMainAIController>(OwnerComp.GetAIOwner());

	if (AIConRef)
	{
		ALibertyPrimeCharacter* IsPawn = Cast<ALibertyPrimeCharacter>(AIConRef->GetPawn());

		if (IsPawn)
		{
			if (AIConRef->NormalAttackCurrent >= AIConRef->NormalAttackTarget)
			{
				int rand = UKismetMathLibrary::RandomIntegerInRange(AIConRef->NormalAttackMin, AIConRef->NormalAttackMax);
				AIConRef->NormalAttackTarget = rand;
				AIConRef->NormalAttackCurrent = 0;
				rand = UKismetMathLibrary::RandomIntegerInRange(AIConRef->CloseRangeAttackMin, AIConRef->CloseRangeAttackMax);
				AIConRef->CloseRangeAttackTarget = rand;
				AIConRef->CloseRangeAttackCurrent = 0;
				rand = UKismetMathLibrary::RandomIntegerInRange(AIConRef->MediumRangeAttackMin, AIConRef->MediumRangeAttackMax);
				AIConRef->MediumRangeAttackTarget = rand;
				AIConRef->MediumRangeAttackCurrent = 0;
				rand = UKismetMathLibrary::RandomIntegerInRange(AIConRef->LongRangeAttackMin, AIConRef->LongRangeAttackMax);
				AIConRef->LongRangeAttackTarget = rand;
				AIConRef->LongRangeAttackCurrent = 0;
			}
		}
	}

	return true;
}


