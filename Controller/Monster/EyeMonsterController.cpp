// Fill out your copyright notice in the Description page of Project Settings.

#include "EyeMonsterController.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Monster/EyeMonster.h"

AEyeMonsterController::AEyeMonsterController()
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

void AEyeMonsterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AEyeMonster* MyPawn = Cast<AEyeMonster>(GetPawn());

	if (MyPawn)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

		if (IsTarget && !IsTarget->IsDead && MyPawn->CheckRestriction())
		{
			float Distance = (MyPawn->GetActorLocation() - IsTarget->GetActorLocation()).Size() - MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() - IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();

			if (Distance >= MyPawn->AttackDistance_Spray_Min && Distance <= MyPawn->AttackDistance_Spray_Max && MediumRangeAttackCurrent < MediumRangeAttackTarget)
			{
				MyPawn->SprayAttack();
				MediumRangeAttackCurrent += 1;
			}
			else if (Distance <= MyPawn->AttackDistance)
			{
				MyPawn->RandomAttack();
				NormalAttackCurrent += 1;
			}
		}

		if (IsTarget && !MyPawn->IsDead && MyPawn->restriction == ERestriction::Restriction_Move_NonRot)
		{
			MoveToActor(IsTarget, IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius() + MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() + 7.5f);
		}
	}
}