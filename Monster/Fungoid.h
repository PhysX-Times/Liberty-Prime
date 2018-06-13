// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "Fungoid.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API AFungoid : public AMonster
{
	GENERATED_BODY()
	
public:

	AFungoid();

	UPROPERTY(EditAnywhere, Category = "CppVariables")
	float AttackDistance_Medium;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> DamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* StingDamager;

	UPROPERTY()
		UAttackData* JumpAttackData;

	void JumpAttack();

	virtual void BeginPlay() override;
	
};
