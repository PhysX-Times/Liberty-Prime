// Fill out your copyright notice in the Description page of Project Settings.

#include "ReaperController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Monster/Reaper.h"

AReaperController::AReaperController()
{
	MediumRangeAttackMax = 2;
	MediumRangeAttackMin = 1;
	MediumRangeAttackTarget = 0;
	MediumRangeAttackCurrent = 0;

	NormalAttackMax = 3;
	NormalAttackMin = 2;
	NormalAttackTarget = 0;
	NormalAttackCurrent = 0;
}

void AReaperController::BeginPlay()
{
	Super::BeginPlay();

	random_skill = UKismetMathLibrary::RandomIntegerInRange(0, 1);
}

void AReaperController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AReaper* MyPawn = Cast<AReaper>(GetPawn());

	if (MyPawn)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

		if (IsTarget && !IsTarget->IsDead && MyPawn->CheckRestriction())
		{
			float Distance = (MyPawn->GetActorLocation() - IsTarget->GetActorLocation()).Size() - MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() - IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();

			if (Distance > MyPawn->AttackDistance && Distance <= MyPawn->AttackDistance_Medium && MediumRangeAttackCurrent < MediumRangeAttackTarget)
			{
				if (random_skill == 1)
				{
					MyPawn->Magic_Target = IsTarget;
					MyPawn->MagicAttack_A();
					random_skill = 0;
				}
				else
				{
					MyPawn->MagicAttack_B();
					random_skill = 1;
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