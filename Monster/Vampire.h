// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "Vampire.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API AVampire : public AMonster
{
	GENERATED_BODY()
	
public:

	AVampire();

	FTimerHandle GasTimer;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Roar;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> ClawDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> GasDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* RightClawDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* LeftClawDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* GasDamager;

	UPROPERTY()
		UAttackData* GasAttack;

	void ExhaleGas();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void GasStart();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void GasEnd();
	void GasInit();
	void GasFunction();

	void ResetDamager_Add() override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
