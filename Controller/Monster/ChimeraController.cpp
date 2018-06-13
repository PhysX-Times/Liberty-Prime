// Fill out your copyright notice in the Description page of Project Settings.

#include "ChimeraController.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Monster/Chimera.h"

AChimeraController::AChimeraController()
{
	MediumRangeAttackMax = 2;
	MediumRangeAttackMin = 1;
	MediumRangeAttackTarget = 0;
	MediumRangeAttackCurrent = 0;
	NormalAttackMax = 3;
	NormalAttackMin = 1;
	NormalAttackTarget = 0;
	NormalAttackCurrent = 0;
}

void AChimeraController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AChimera* MyPawn = Cast<AChimera>(GetPawn());

	if (MyPawn)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

		if (IsTarget && !IsTarget->IsDead && MyPawn->CheckRestriction())
		{
			float Distance = (MyPawn->GetActorLocation() - IsTarget->GetActorLocation()).Size() - MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() - IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();

			if (Distance > MyPawn->AttackDistance && Distance <= MyPawn->AttackDistance_Medium && MediumRangeAttackCurrent < MediumRangeAttackTarget)
			{
				MyPawn->RamAttack();

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