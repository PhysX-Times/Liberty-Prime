// Fill out your copyright notice in the Description page of Project Settings.

#include "FindNextPoint.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Controller/MainAIController.h"
#include "LibertyPrimeCharacter.h"
#include "Miscellaneous/PathIndicator.h"

EBTNodeResult::Type UFindNextPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMainAIController* AIConRef = Cast<AMainAIController>(OwnerComp.GetAIOwner());

	if (AIConRef)
	{
		ALibertyPrimeCharacter* IsPawn = Cast<ALibertyPrimeCharacter>(AIConRef->GetPawn());
		FVector NextLocation = IsPawn->TargetPath->GetActorLocation();
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(AIConRef->NextLocationKeyID, NextLocation);
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(AIConRef->WaitKeyID, IsPawn->TargetPath->CanWait);
		IsPawn->TargetPath = IsPawn->TargetPath->NextTarget;

		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}
