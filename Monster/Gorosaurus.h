// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "Gorosaurus.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API AGorosaurus : public AMonster
{
	GENERATED_BODY()
	
public:

	AGorosaurus();

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> ClawDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* RightClawDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* LeftClawDamager;

	virtual void BeginPlay() override;
	
};
