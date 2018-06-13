// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "Ghoul.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API AGhoul : public AMonster
{
	GENERATED_BODY()
	
public:

	AGhoul();

	UPROPERTY(EditAnywhere, Category = "CppVariables")
	float AttackDistance_Medium;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> HandDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* RightHandDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* LeftHandDamager;

	UPROPERTY()
		UAttackData* JumpAttackData;

	void JumpAttack();
	
	virtual void BeginPlay() override;
	
};
