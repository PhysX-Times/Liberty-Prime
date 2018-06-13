// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "Griffin.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API AGriffin : public AMonster
{
	GENERATED_BODY()
	
	
public:

	AGriffin();

	UPROPERTY(EditAnywhere, Category = "SubClasses")
	TSubclassOf<ASword> ClawDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
	TSubclassOf<ASword> BleakDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* RightClawDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* LeftClawDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* BleakDamager;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		UParticleSystem* TailPS;

	virtual void BeginPlay() override;
	
};
