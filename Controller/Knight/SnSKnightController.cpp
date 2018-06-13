// Fill out your copyright notice in the Description page of Project Settings.

#include "SnSKnightController.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Knight/SnSKnight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

ASnSKnightController::ASnSKnightController()
{
	LongRangeAttackMax = 2;
	LongRangeAttackMin = 0;
	LongRangeAttackTarget = 0;
	LongRangeAttackCurrent = 0;
	MediumRangeAttackMax = 2;
	MediumRangeAttackMin = 0;
	MediumRangeAttackTarget = 0;
	MediumRangeAttackCurrent = 0;
	CloseRangeAttackMax = 2;
	CloseRangeAttackMin = 0;
	CloseRangeAttackTarget = 0;
	CloseRangeAttackCurrent = 0;
	NormalAttackMax = 3;
	NormalAttackMin = 1;
	NormalAttackTarget = 0;
	NormalAttackCurrent = 0;
}

void ASnSKnightController::BeginPlay()
{
	Super::BeginPlay();

}

void ASnSKnightController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ASnSKnight* MyPawn = Cast<ASnSKnight>(GetPawn());

	if (MyPawn)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

		if (IsTarget)
		{
			if (MyPawn->CheckRestriction())
			{
				float Distance = (MyPawn->GetActorLocation() - IsTarget->GetActorLocation()).Size() - MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() - IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();

				if (Distance >= MyPawn->AttackDistance_Long_Min && Distance < MyPawn->AttackDistance_Long && LongRangeAttackCurrent < LongRangeAttackTarget)
				{
					MyPawn->EnergyAttack();
					LongRangeAttackCurrent += 1;
				}
				else if (Distance > MyPawn->AttackDistance_Close && Distance <= MyPawn->AttackDistance_Medium && MediumRangeAttackCurrent < MediumRangeAttackTarget)
				{
					MyPawn->DashAttack();
					MediumRangeAttackCurrent += 1;
				}
				else if (Distance > MyPawn->AttackDistance && Distance <= MyPawn->AttackDistance_Close && CloseRangeAttackCurrent < CloseRangeAttackTarget)
				{
					MyPawn->SpinAttack();
					CloseRangeAttackCurrent += 1;
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