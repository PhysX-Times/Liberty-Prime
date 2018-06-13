// Fill out your copyright notice in the Description page of Project Settings.

#include "DragonWormController.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Monster/DragonWorm.h"
#include "Engine.h"

ADragonWormController::ADragonWormController()
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

void ADragonWormController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ADragonWorm* MyPawn = Cast<ADragonWorm>(GetPawn());

	if (MyPawn)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

		if (IsTarget && !IsTarget->IsDead && MyPawn->CheckRestriction())
		{
			MyPawn->bFocus = true;

			float Capsule_Sum = MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() + IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();
			float Distance = (MyPawn->GetActorLocation() - IsTarget->GetActorLocation()).Size() - Capsule_Sum;

			if (Distance <= MyPawn->AttackDistance)
			{
				MyPawn->RandomAttack();
			}
			else if (Distance > MyPawn->AttackDistance + Capsule_Sum + 175.0f)
			{
				MyPawn->EQS_Radius = Capsule_Sum + MyPawn->AttackDistance - 12.5f;
				MyPawn->Disappear();
			}
		}
		else
		{
			MyPawn->bFocus = false;
		}
	}
}