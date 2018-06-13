// Fill out your copyright notice in the Description page of Project Settings.

#include "MainAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Controller/MainAIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "LibertyPrimeCharacter.h"
#include "Engine.h"

AMainAIController::AMainAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
}

void AMainAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ALibertyPrimeCharacter* MyPawn = Cast<ALibertyPrimeCharacter>(GetPawn());

	if (MyPawn && !MyPawn->IsDead && (MyPawn->restriction == ERestriction::Restriction_Rot || MyPawn->restriction == ERestriction::Restriction_Move || MyPawn->bFocus))
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));

		if (IsTarget)
		{
			MyPawn->LerpRot = UKismetMathLibrary::FindLookAtRotation(MyPawn->GetActorLocation(), IsTarget->GetActorLocation());;
		}
		else
		{
			MyPawn->LerpRot = MyPawn->GetActorRotation();
		}
	}
}

void AMainAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	ALibertyPrimeCharacter* IsChar = Cast<ALibertyPrimeCharacter>(InPawn);

	if (IsChar && IsChar->BotBehavior)
	{
		BlackboardComp->InitializeBlackboard(*IsChar->BotBehavior->BlackboardAsset);
		BehaviorTreeComp->StartTree(*IsChar->BotBehavior);

		EnemyKeyID = BlackboardComp->GetKeyID("Target");
		NextLocationKeyID = BlackboardComp->GetKeyID("NextLocation");
		WaitKeyID = BlackboardComp->GetKeyID("IsWait");
	}
}