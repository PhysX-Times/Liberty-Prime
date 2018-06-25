// Fill out your copyright notice in the Description page of Project Settings.

#include "BoneDragon_TwinController.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Monster/BoneDragon_Twin.h"


ABoneDragon_TwinController::ABoneDragon_TwinController()
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

void ABoneDragon_TwinController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ABoneDragon_Twin* MyPawn = Cast<ABoneDragon_Twin>(GetPawn());

	if (MyPawn)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

		if (IsTarget && !IsTarget->IsDead)
		{
			if (MyPawn->CheckRestriction())
			{
				float Distance = (MyPawn->GetActorLocation() - IsTarget->GetActorLocation()).Size() - MyPawn->GetCapsuleComponent()->GetScaledCapsuleRadius() - IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();

				if (Distance <= MyPawn->AttackDistance)
				{
					FVector Offset = (IsTarget->GetActorLocation() - MyPawn->GetActorLocation()).GetSafeNormal();

					float DotProduct = FVector::DotProduct(MyPawn->GetActorForwardVector(), Offset);

					if (DotProduct < 0.2f)
					{
						DotProduct = FVector::DotProduct(MyPawn->GetActorRightVector(), Offset);

						if (DotProduct < 0.0f)
						{
							MyPawn->Attack_Tail_Left();
						}
						else
						{
							MyPawn->Attack_Tail_Right();
						}
					}
					else
					{
						MyPawn->Attack_Bite();
					}
				}
			}
		}
	}
}