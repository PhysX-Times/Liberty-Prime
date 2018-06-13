// Fill out your copyright notice in the Description page of Project Settings.

#include "CaveCrawlerController.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Monster/CaveCrawler.h"

ACaveCrawlerController::ACaveCrawlerController()
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

void ACaveCrawlerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ACaveCrawler* MyPawn = Cast<ACaveCrawler>(GetPawn());

	if (MyPawn)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

		if (IsTarget && !IsTarget->IsDead)
		{
			if (MyPawn->CheckRestriction())
			{
				float Distance = (MyPawn->GetActorLocation() - IsTarget->GetActorLocation()).Size() - MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() - IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();

				if (Distance > MyPawn->AttackDistance && Distance <= MyPawn->AttackDistance_Gas && MediumRangeAttackCurrent < MediumRangeAttackTarget)
				{
					MyPawn->ExhaleGas();

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
}