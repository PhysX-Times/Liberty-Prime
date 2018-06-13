// Fill out your copyright notice in the Description page of Project Settings.

#include "DragonRexController.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Monster/DragonRex.h"

ADragonRexController::ADragonRexController()
{
	MediumRangeAttackMax = 3;
	MediumRangeAttackMin = 1;
	MediumRangeAttackTarget = 0;
	MediumRangeAttackCurrent = 0;
	NormalAttackMax = 3;
	NormalAttackMin = 1;
	NormalAttackTarget = 0;
	NormalAttackCurrent = 0;
}

void ADragonRexController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ADragonRex* MyPawn = Cast<ADragonRex>(GetPawn());

	if (MyPawn && MyPawn->CheckRestriction())
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

		if (MyPawn)
		{
			ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

			if (IsTarget && !IsTarget->IsDead && MyPawn->CheckRestriction())
			{
				float Distance = (MyPawn->GetActorLocation() - IsTarget->GetActorLocation()).Size() - MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() - IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();

				if (Distance > MyPawn->AttackDistance_Bite && Distance <= MyPawn->AttackDistance_FireBreathe && MediumRangeAttackCurrent < MediumRangeAttackTarget)
				{
					MyPawn->FlameThrow();
					MediumRangeAttackCurrent += 1;
				}
				else if (Distance > MyPawn->AttackDistance && Distance <= MyPawn->AttackDistance_Bite)
				{
					MyPawn->BiteAttack();
					NormalAttackCurrent += 1;
				}
				else if (Distance <= MyPawn->AttackDistance)
				{
					MyPawn->RandomAttack();
					NormalAttackCurrent += 1;
				}
			}
		}
	}
}