// Fill out your copyright notice in the Description page of Project Settings.

#include "GiantCrabController.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Monster/GiantCrab.h"

AGiantCrabController::AGiantCrabController()
{
	NormalAttackMax = 3;
	NormalAttackMin = 1;
	NormalAttackTarget = 0;
	NormalAttackCurrent = 0;
}

void AGiantCrabController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AGiantCrab* MyPawn = Cast<AGiantCrab>(GetPawn());

	if (MyPawn)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

		if (IsTarget && !IsTarget->IsDead && MyPawn->CheckRestriction())
		{
			float Distance = (MyPawn->GetActorLocation() - IsTarget->GetActorLocation()).Size() - MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() - IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();

			if (Distance <= MyPawn->AttackDistance)
			{
				MyPawn->RandomAttack();
			}
		}
	}
}