// Fill out your copyright notice in the Description page of Project Settings.

#include "GreaterSpiderController.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Monster/GreaterSpider.h"

AGreaterSpiderController::AGreaterSpiderController()
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

void AGreaterSpiderController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AGreaterSpider* MyPawn = Cast<AGreaterSpider>(GetPawn());

	if (MyPawn)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

		if (IsTarget && !IsTarget->IsDead && MyPawn->CheckRestriction())
		{
			float Distance = (MyPawn->GetActorLocation() - IsTarget->GetActorLocation()).Size() - MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() - IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();

			if (Distance >= MyPawn->AttackDistance_Charge_Min && Distance <= MyPawn->AttackDistance_Charge_Max)
			{
				MyPawn->ChargeAttack();
			}
			else if (Distance <= MyPawn->AttackDistance)
			{
				MyPawn->RandomAttack();
			}
		}
	}
}