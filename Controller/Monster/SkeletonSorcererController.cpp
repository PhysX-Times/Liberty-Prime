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
	CloseRangeAttackMax = 2;
	CloseRangeAttackMin = 0;
	CloseRangeAttackTarget = 0;
	CloseRangeAttackCurrent = 0;
	NormalAttackMax = 2;
	NormalAttackMin = 0;
	NormalAttackTarget = 0;
	NormalAttackCurrent = 0;
}

void ASkeletonSorcererController::BeginPlay()
{
	Super::BeginPlay();

	rand_skill = UKismetMathLibrary::RandomIntegerInRange(0, 2);
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

				if (Distance > MyPawn->AttackDistance && Distance <= MyPawn->AttackDistance_Magic && MyPawn->can_cast)
				{
					switch (rand_skill)
					{
					case 0:
						MyPawn->Magic_Staff();
						rand_skill = 1;
						break;
					case 1:
						MyPawn->Magic_Arrow();
						rand_skill = 2;
						break;
					case 2:
						MyPawn->Magic_Shell();
						rand_skill = 0;
						break;
					}
				}
				else if (Distance <= MyPawn->AttackDistance)
				{
					if (CloseRangeAttackCurrent < CloseRangeAttackTarget)
					{
						MyPawn->Magic_Target = IsTarget;
						MyPawn->Magic_Teleport();
						CloseRangeAttackCurrent += 1;
					}
					else
					{
						MyPawn->RandomAttack();
						NormalAttackCurrent += 1;
					}
				}
			}
		}
	}
}