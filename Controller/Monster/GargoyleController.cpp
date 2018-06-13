// Fill out your copyright notice in the Description page of Project Settings.

#include "GargoyleController.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Monster/Gargoyle.h"
#include "Engine.h"

AGargoyleController::AGargoyleController()
{
	MediumRangeAttackMax = 4;
	MediumRangeAttackMin = 1;
	MediumRangeAttackTarget = 0;
	MediumRangeAttackCurrent = 0;
	NormalAttackMax = 3;
	NormalAttackMin = 1;
	NormalAttackTarget = 0;
	NormalAttackCurrent = 0;
	CloseRangeAttackMax = 1;
	CloseRangeAttackMin = 0;
	CloseRangeAttackTarget = 0;
	CloseRangeAttackCurrent = 0;
}

void AGargoyleController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AGargoyle* MyPawn = Cast<AGargoyle>(GetPawn());

	if (MyPawn)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

		if (IsTarget && !IsTarget->IsDead)
		{
			if (MyPawn->CheckRestriction())
			{
				float Distance = (MyPawn->GetActorLocation() - IsTarget->GetActorLocation()).Size() - MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() - IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();

				if (Distance >= MyPawn->FlyDistance && !MyPawn->bIsFlyMode)
				{
					MyPawn->FlyFunc();
				}
				else if (Distance > MyPawn->LandDistance_Max && Distance < MyPawn->FlyDistance && MediumRangeAttackCurrent < MediumRangeAttackTarget)
				{
					if (MyPawn->bIsFlyMode)
					{
						MyPawn->FlyMode_Cast();
						MyPawn->DMGType = EDamageType::Damage_Fire;
					}
					else
					{
						MyPawn->FlameAttack();
					}

					MediumRangeAttackCurrent += 1;
				}
				else if (MyPawn->bIsFlyMode && Distance >= MyPawn->LandDistance_Min && Distance <= MyPawn->LandDistance_Max && CloseRangeAttackCurrent < CloseRangeAttackTarget)
				{
					MyPawn->LandFunc();
					CloseRangeAttackCurrent += 1;
				}
				else if (Distance <= MyPawn->AttackDistance)
				{
					MyPawn->RandomAttack();
					MyPawn->DMGType = EDamageType::Damage_None;
					NormalAttackCurrent += 1;
				}
			}

			if (IsTarget && !IsTarget->IsDead && MyPawn->restriction == ERestriction::Restriction_Move && !MyPawn->IsDead)
			{
				MoveToActor(IsTarget, IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius() + MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() + 7.5f);
			}
		}
	}
}