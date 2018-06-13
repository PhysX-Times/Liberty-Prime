// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "Wyvern.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API AWyvern : public AMonster
{
	GENERATED_BODY()
	
public:

	AWyvern();

	FTimerHandle FlameTimer;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Breathe;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Stinger;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_FireBreathe;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> BiteDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> StingerDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> FlameDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* BiteDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* StingerDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* FlameDamager;

	UPROPERTY()
		UAttackData* FlameThrowAttack;

	void FlameThrow();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void FlameStart();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void FlameEnd();
	void FlameInit();
	void FlameFunction();

	void ResetDamager_Add() override;
	void ResetMontage_Add() override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
