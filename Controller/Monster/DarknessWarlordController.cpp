// Fill out your copyright notice in the Description page of Project Settings.

#include "DarknessWarlordController.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Monster/DarknessWarlord.h"

ADarknessWarlordController::ADarknessWarlordController()
{
	NormalAttackMax = 5;
	NormalAttackMin = 2;
	NormalAttackTarget = 0;
	NormalAttackCurrent = 0;
}

void ADarknessWarlordController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ADarknessWarlord* MyPawn = Cast<ADarknessWarlord>(GetPawn());

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