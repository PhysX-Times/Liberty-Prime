// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MainAIController.generated.h"

/**
*
*/
UCLASS()
class LIBERTYPRIME_API AMainAIController : public AAIController
{
	GENERATED_BODY()

		UPROPERTY(transient)
		class UBlackboardComponent* BlackboardComp;
	UPROPERTY(transient)
		class UBehaviorTreeComponent* BehaviorTreeComp;

public:

	AMainAIController();

	int NormalAttackMax;
	int NormalAttackMin;
	int NormalAttackTarget;
	int NormalAttackCurrent;
	int LongRangeAttackMax;
	int LongRangeAttackMin;
	int LongRangeAttackTarget;
	int LongRangeAttackCurrent;
	int MediumRangeAttackMax;
	int MediumRangeAttackMin;
	int MediumRangeAttackTarget;
	int MediumRangeAttackCurrent;
	int CloseRangeAttackMax;
	int CloseRangeAttackMin;
	int CloseRangeAttackTarget;
	int CloseRangeAttackCurrent;

	uint8 EnemyKeyID;
	uint8 NextLocationKeyID;
	uint8 WaitKeyID;

	virtual void Possess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class UBlackboardComponent* GetBlackboardComp() { return BlackboardComp; }
};
