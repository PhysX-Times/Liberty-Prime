// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "GiantCrab.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API AGiantCrab : public AMonster
{
	GENERATED_BODY()
	
	
public:

	AGiantCrab();

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> ClawDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* RightClawDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* LeftClawDamager;

	virtual void BeginPlay() override;

	
};
