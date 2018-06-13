// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "MonsterBoss.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API AMonsterBoss : public AMonster
{
	GENERATED_BODY()
	
public:

	AMonsterBoss();

	TArray<ALibertyPrimeCharacter*> DamagedChars;

	FTimerHandle FlameTimer;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Jump_Max;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Jump_Min;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Breathe_Min;

	bool bFireBreathe;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Smash;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Impact;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_FireBreathe;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> FlameDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* FlameDamager;

	UPROPERTY()
		UAttackData* JumpAttackData;
	UPROPERTY()
		UAttackData* FlameThrowAttack;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Smash_PS;
	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Impact_PS;
	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* FlamePS_End;

	UPROPERTY()
		UParticleSystemComponent* FirstPS_End_Comp;

	void FlameThrow();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void FlameStart();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void FlameEnd();
	void FlameInit();
	void FlameFunction();

	void ResetDamager_Add() override;

	void JumpAttack();
	void Blast(FName TraceName, float radius, UParticleSystem* Target_PS, USoundCue* Target_Cue, FVector Target_Scale, EWeaponType weapon_type);

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Smash();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Claw_Impact(FName BoneName);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
