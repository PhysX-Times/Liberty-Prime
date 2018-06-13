// Fill out your copyright notice in the Description page of Project Settings.

#include "GiantWormController.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Monster/GiantWorm.h"

AGiantWormController::AGiantWormController()
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

void AGiantWormController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AGiantWorm* MyPawn = Cast<AGiantWorm>(GetPawn());

	if (MyPawn)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

		if (IsTarget && !IsTarget->IsDead && MyPawn->CheckRestriction())
		{
			MyPawn->bFocus = true;

			float Capsule_Sum = MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() + IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();
			float Distance = (MyPawn->GetActorLocation() - IsTarget->GetActorLocation()).Size() - Capsule_Sum;

			if (Distance <= MyPawn->AttackDistance_Circle)
			{
				MyPawn->CircleAttack();
			}
			else if (Distance > MyPawn->AttackDistance_Circle && Distance <= MyPawn->AttackDistance_Random)
			{
				MyPawn->RandomAttack();
			}
			else if (Distance > MyPawn->AttackDistance_Random && Distance <= MyPawn->AttackDistance)
			{
				MyPawn->BreatheAttack();
			}
			else if (Distance > MyPawn->AttackDistance + Capsule_Sum + 125.0f)
			{
				MyPawn->EQS_Radius = Capsule_Sum + MyPawn->AttackDistance_Circle - 12.5f;
				MyPawn->Disappear();
			}
		}
		else
		{
			MyPawn->bFocus = false;
		}
	}
}