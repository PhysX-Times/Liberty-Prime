// Fill out your copyright notice in the Description page of Project Settings.

#include "CyclopController.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Kismet/KismetMathLibrary.h"
#include "Monster/Cyclop.h"

ACyclopController::ACyclopController()
{
	MediumRangeAttackMax = 0;
	MediumRangeAttackMin = 0;
	MediumRangeAttackTarget = 0;
	MediumRangeAttackCurrent = 0;
	NormalAttackMax = 3;
	NormalAttackMin = 1;
	NormalAttackTarget = 0;
	NormalAttackCurrent = 0;
}

void ACyclopController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ACyclop* MyPawn = Cast<ACyclop>(GetPawn());

	if (MyPawn)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

		if (IsTarget && !IsTarget->IsDead && MyPawn->CheckRestriction())
		{
			float Distance = (MyPawn->GetActorLocation() - IsTarget->GetActorLocation()).Size() - MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() - IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();

			if (Distance <= MyPawn->AttackDistance)
			{
				MyPawn->RandomAttack();
			}
		}
	}
}