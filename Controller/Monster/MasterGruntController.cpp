// Fill out your copyright notice in the Description page of Project Settings.

#include "MasterGruntController.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Monster/MasterGrunt.h"

AMasterGruntController::AMasterGruntController()
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

void AMasterGruntController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AMasterGrunt* MyPawn = Cast<AMasterGrunt>(GetPawn());

	if (MyPawn && MyPawn->CheckRestriction())
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

		if (MyPawn)
		{
			ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

			if (IsTarget && !IsTarget->IsDead && MyPawn->CheckRestriction())
			{
				float Distance = (MyPawn->GetActorLocation() - IsTarget->GetActorLocation()).Size() - MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() - IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();

				if (Distance >= MyPawn->AttackDistance_Cannon_Min && Distance <= MyPawn->AttackDistance_Cannon_Max)
				{
					MyPawn->Cannon_Target = IsTarget;
					MyPawn->DMGType = EDamageType::Damage_Fire;
					MyPawn->CannonAttack();
				}
				else if (Distance >= MyPawn->AttackDistance_Heal_Min && Distance < MyPawn->AttackDistance_Cannon_Min && MediumRangeAttackCurrent < MediumRangeAttackTarget)
				{
					MyPawn->Cast_Heal();
					MediumRangeAttackCurrent += 1;
				}
				else if (Distance <= MyPawn->AttackDistance)
				{
					MyPawn->DMGType = EDamageType::Damage_None;
					MyPawn->RandomAttack();
					NormalAttackCurrent += 1;
				}
			}
		}
	}
}