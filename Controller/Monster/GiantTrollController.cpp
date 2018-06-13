// Fill out your copyright notice in the Description page of Project Settings.

#include "GiantTrollController.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Monster/GiantTroll.h"

AGiantTrollController::AGiantTrollController()
{
	MediumRangeAttackMax = 2;
	MediumRangeAttackMin = 0;
	MediumRangeAttackTarget = 0;
	MediumRangeAttackCurrent = 0;
	NormalAttackMax = 3;
	NormalAttackMin = 1;
	NormalAttackTarget = 0;
	NormalAttackCurrent = 0;
}

void AGiantTrollController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AGiantTroll* MyPawn = Cast<AGiantTroll>(GetPawn());

	if (MyPawn)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

		if (IsTarget && !IsTarget->IsDead && MyPawn->CheckRestriction())
		{
			float Distance = (MyPawn->GetActorLocation() - IsTarget->GetActorLocation()).Size() - MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() - IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();

			if (Distance <= MyPawn->AttackDistance_Smash && MediumRangeAttackCurrent < MediumRangeAttackTarget)
			{
				MyPawn->SmashAttack();

				MediumRangeAttackCurrent += 1;
			}
			else if (Distance <= MyPawn->AttackDistance)
			{
				MyPawn->RandomAttack();
				NormalAttackCurrent += 1;
			}
		}
	}
}