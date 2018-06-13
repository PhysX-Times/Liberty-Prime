// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "Cyclop.generated.h"

class ASword;
class UAttackData;


UCLASS()
class LIBERTYPRIME_API ACyclop : public AMonster
{
	GENERATED_BODY()
	
public:

	ACyclop();

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> HandDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> LegDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* HandDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* LegDamager;

	virtual void BeginPlay() override;
	
};
