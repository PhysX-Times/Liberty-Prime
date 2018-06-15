// Fill out your copyright notice in the Description page of Project Settings.

#include "SkeletonSorcererController.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Monster/SkeletonSorcerer.h"
#include "Kismet/KismetMathLibrary.h"

ASkeletonSorcererController::ASkeletonSorcererController()
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

void ASkeletonSorcererController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ASkeletonSorcerer* MyPawn = Cast<ASkeletonSorcerer>(GetPawn());

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

				if (Distance > MyPawn->AttackDistance && Distance <= MyPawn->AttackDistance_Magic)
				{
					MyPawn->Magic_Arrow();
					
					/*
					int rand = UKismetMathLibrary::RandomIntegerInRange(0, 2);

					switch (rand)
					{
					case 0:
						MyPawn->Magic_Staff();
						break;
					case 1:
						MyPawn->Magic_Summon();
						break;
					case 2:
						MyPawn->Magic_Arrow();
						break;
					}
					*/
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