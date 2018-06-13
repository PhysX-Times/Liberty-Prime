// Fill out your copyright notice in the Description page of Project Settings.

#include "WormBossController.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Monster/WormBoss.h"

AWormBossController::AWormBossController()
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

void AWormBossController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AWormBoss* MyPawn = Cast<AWormBoss>(GetPawn());

	if (MyPawn && MyPawn->CheckRestriction())
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

		if (IsTarget && !IsTarget->IsDead)
		{
			float Capsule_Sum = MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() + IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();
			float Distance = (MyPawn->GetActorLocation() - IsTarget->GetActorLocation()).Size() - Capsule_Sum;

			MyPawn->Target = IsTarget;

			if (Distance <= MyPawn->AttackDistance)
			{
				int rand = UKismetMathLibrary::RandomIntegerInRange(0, 5);

				if (rand == 5)
				{
					MyPawn->Disappear();
				}
				else
				{
					MyPawn->RandomAttack();
				}
			}
			else if (Distance > MyPawn->AttackDistance && Distance <= MyPawn->AttackDistance_Breathe)
			{
				int rand = UKismetMathLibrary::RandomIntegerInRange(0, 3);

				if (rand == 3)
				{
					MyPawn->Disappear();
				}
				else
				{
					MyPawn->BreatheAttack();
				}
			}
			else if (Distance > MyPawn->AttackDistance_Breathe + Capsule_Sum + 125.0f)
			{
				MyPawn->Disappear();
			}
		}
	}
}