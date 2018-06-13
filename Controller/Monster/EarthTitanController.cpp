// Fill out your copyright notice in the Description page of Project Settings.

#include "EarthTitanController.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Monster/EarthTitan.h"

AEarthTitanController::AEarthTitanController()
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

void AEarthTitanController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AEarthTitan* MyPawn = Cast<AEarthTitan>(GetPawn());

	if (MyPawn)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

		if (IsTarget && !IsTarget->IsDead && MyPawn->CheckRestriction())
		{
			float Distance = (MyPawn->GetActorLocation() - IsTarget->GetActorLocation()).Size() - MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() - IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();

			if (Distance <= MyPawn->AttackDistance)
			{
				MyPawn->DMGType = EDamageType::Damage_None;
				MyPawn->RandomAttack();
			}

			if (Distance > MyPawn->AttackDistance && Distance <= MyPawn->AttackDistance_Min)
			{
				int rand = UKismetMathLibrary::RandomIntegerInRange(0, 1);

				if (rand == 0)
				{
					MyPawn->DMGType = EDamageType::Damage_None;
					MyPawn->ComboAttack();
				}
				else
				{
					MyPawn->DMGType = EDamageType::Damage_Fire;
					MyPawn->SmashAttack();
				}
			}
		}
	}
}