// Fill out your copyright notice in the Description page of Project Settings.

#include "ElongataController.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Monster/Elongata.h"

AElongataController::AElongataController()
{
	MediumRangeAttackMax = 1;
	MediumRangeAttackMin = 1;
	MediumRangeAttackTarget = 0;
	MediumRangeAttackCurrent = 0;
	NormalAttackMax = 2;
	NormalAttackMin = 1;
	NormalAttackTarget = 0;
	NormalAttackCurrent = 0;
}

void AElongataController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AElongata* MyPawn = Cast<AElongata>(GetPawn());

	if (MyPawn)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

		if (IsTarget && !IsTarget->IsDead && MyPawn->CheckRestriction())
		{
			float Capsule_Sum = MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() + IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();
			float Distance = (MyPawn->GetActorLocation() - IsTarget->GetActorLocation()).Size() - Capsule_Sum;

			MyPawn->bFocus = true;

			if (Distance > MyPawn->AttackDistance && Distance <= MyPawn->AttackDistance_Spit && MediumRangeAttackCurrent < MediumRangeAttackTarget)
			{
				MyPawn->SpitAttack();
				MediumRangeAttackCurrent += 1;
			}
			else if (Distance <= MyPawn->AttackDistance)
			{
				MyPawn->RandomAttack();
				NormalAttackCurrent += 1;
			}
			else if (Distance > MyPawn->AttackDistance + Capsule_Sum + 175.0f)
			{
				MyPawn->EQS_Radius = Capsule_Sum + MyPawn->AttackDistance - 12.5f;
				MyPawn->Disappear();
			}
		}
		else
		{
			MyPawn = false;
		}
	}
}