// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterBossController.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Monster/MonsterBoss.h"

AMonsterBossController::AMonsterBossController()
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

void AMonsterBossController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AMonsterBoss* MyPawn = Cast<AMonsterBoss>(GetPawn());

	if (MyPawn)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

		if (IsTarget && !IsTarget->IsDead && MyPawn->CheckRestriction())
		{
			float Distance = (MyPawn->GetActorLocation() - IsTarget->GetActorLocation()).Size() - MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() - IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();

			if (Distance >= MyPawn->AttackDistance_Breathe_Min && Distance < MyPawn->AttackDistance_Jump_Min && MediumRangeAttackCurrent < MediumRangeAttackTarget)
			{
				MyPawn->FlameThrow();
				MediumRangeAttackCurrent += 1;
			}
			else if (Distance >= MyPawn->AttackDistance_Jump_Min && Distance <= MyPawn->AttackDistance_Jump_Max)
			{
				MyPawn->JumpAttack();
			}
			else if (Distance <= MyPawn->AttackDistance)
			{
				MyPawn->RandomAttack();
				NormalAttackCurrent += 1;
			}
		}
	}
}