// Fill out your copyright notice in the Description page of Project Settings.

#include "TrollController.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Kismet/KismetMathLibrary.h"
#include "Monster/Troll.h"

ATrollController::ATrollController()
{
	LongRangeAttackMax = 2;
	LongRangeAttackMin = 0;
	LongRangeAttackTarget = 0;
	LongRangeAttackCurrent = 0;
	MediumRangeAttackMax = 3;
	MediumRangeAttackMin = 1;
	MediumRangeAttackTarget = 0;
	MediumRangeAttackCurrent = 0;
	NormalAttackMax = 3;
	NormalAttackMin = 1;
	NormalAttackTarget = 0;
	NormalAttackCurrent = 0;
}

void ATrollController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ATroll* MyPawn = Cast<ATroll>(GetPawn());

	if (MyPawn)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

		if (IsTarget && !IsTarget->IsDead && MyPawn->CheckRestriction())
		{
			float Distance = (MyPawn->GetActorLocation() - IsTarget->GetActorLocation()).Size() - MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() - IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();

			if (Distance <= MyPawn->AttackDistance_Long && LongRangeAttackCurrent < LongRangeAttackTarget)
			{
				MyPawn->DashAttack();
				LongRangeAttackCurrent += 1;
			}
			else if (Distance <= MyPawn->AttackDistance_Medium && MediumRangeAttackCurrent < MediumRangeAttackTarget)
			{
				int rand = UKismetMathLibrary::RandomIntegerInRange(0, 2);

				switch (rand)
				{
				case 0:
					MyPawn->Roar();
					break;
				case 1:
					MyPawn->SwipeAttack();
					break;
				case 2:
					MyPawn->HeadOverAttack();
					break;
				}

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