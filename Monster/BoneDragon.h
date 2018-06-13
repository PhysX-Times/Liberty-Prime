// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "BoneDragon.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API ABoneDragon : public AMonster
{
	GENERATED_BODY()
	
public:

	ABoneDragon();

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
	float OffsetYaw;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	float AttackDistance_FireBreathe;

	FRotator TargetRot;

	FTimerHandle FlameTimer;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> HandDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> BiteDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> FlameDamagerClass;
	
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* RightHandDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* LeftHandDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* BiteDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* FlameDamager;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_FireBreathe;

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

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
