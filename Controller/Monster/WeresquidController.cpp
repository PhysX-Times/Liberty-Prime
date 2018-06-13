// Fill out your copyright notice in the Description page of Project Settings.

#include "WeresquidController.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Monster/Weresquid.h"

AWeresquidController::AWeresquidController()
{
	MediumRangeAttackMax = 3;
	MediumRangeAttackMin = 1;
	MediumRangeAttackTarget = 0;
	MediumRangeAttackCurrent = 0;
	CloseRangeAttackMax = 3;
	CloseRangeAttackMin = 1;
	CloseRangeAttackTarget = 0;
	CloseRangeAttackCurrent = 0;
	NormalAttackMax = 3;
	NormalAttackMin = 1;
	NormalAttackTarget = 0;
	NormalAttackCurrent = 0;
}

void AWeresquidController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AWeresquid* MyPawn = Cast<AWeresquid>(GetPawn());

	if (MyPawn && MyPawn->CheckRestriction())
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

		if (MyPawn)
		{
			ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

			if (IsTarget && !IsTarget->IsDead && MyPawn->CheckRestriction())
			{
				float Distance = (MyPawn->GetActorLocation() - IsTarget->GetActorLocation()).Size() - MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() - IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();
				MyPawn->Magic_Target = IsTarget;

				if (Distance > MyPawn->AttackDistance && Distance <= MyPawn->AttackDistance_Staff)
				{
					MyPawn->Magic_Staff();
				}
				else if (Distance <= MyPawn->AttackDistance)
				{
					MyPawn->Magic_Target = IsTarget;
					MyPawn->Magic_Teleport();
				}
			}
		}
	}
}