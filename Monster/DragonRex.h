// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "DragonRex.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API ADragonRex : public AMonster
{
	GENERATED_BODY()
	
public:

	ADragonRex();

	TArray<ALibertyPrimeCharacter*> DamagedChars;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_FireBreathe;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Bite;

	bool bFireBreathe;

	FRotator TargetRot;

	FTimerHandle FlameTimer;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> TailDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> BiteDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> FlameDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* TailDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* BiteDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* FlameDamager;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_FireBreathe;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* FlamePS_End;

	UPROPERTY()
		UAttackData* FlameThrowAttack;
	UPROPERTY()
		UAttackData* BiteAttackData;

	UPROPERTY()
		UParticleSystemComponent* FirstPS_End_Comp;

	void FlameThrow();
	void BiteAttack();

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
