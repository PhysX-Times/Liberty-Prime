// Fill out your copyright notice in the Description page of Project Settings.

#include "FungoidController.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Monster/Fungoid.h"

AFungoidController::AFungoidController()
{
	MediumRangeAttackMax = 2;
	MediumRangeAttackMin = 1;
	MediumRangeAttackTarget = 0;
	MediumRangeAttackCurrent = 0;
	NormalAttackMax = 4;
	NormalAttackMin = 2;
	NormalAttackTarget = 0;
	NormalAttackCurrent = 0;
}

void AFungoidController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AFungoid* MyPawn = Cast<AFungoid>(GetPawn());

	if (MyPawn)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

		if (IsTarget && !IsTarget->IsDead && MyPawn->CheckRestriction())
		{
			float Distance = (MyPawn->GetActorLocation() - IsTarget->GetActorLocation()).Size() - MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() - IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();

			if (Distance <= MyPawn->AttackDistance_Medium && MediumRangeAttackCurrent < MediumRangeAttackTarget)
			{
				MyPawn->JumpAttack();

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