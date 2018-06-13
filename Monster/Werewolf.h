// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "Werewolf.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API AWerewolf : public AMonster
{
	GENERATED_BODY()
	
public:

	AWerewolf();

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Medium;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
	TSubclassOf<ASword> ClawDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
	TSubclassOf<ASword> BiteDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
	ASword* RightClawDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
	ASword* LeftClawDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
	ASword* BiteDamager;

	UPROPERTY()
	UAttackData* JumpAttackData;

	void JumpAttack();

	virtual void BeginPlay() override;
	
};
