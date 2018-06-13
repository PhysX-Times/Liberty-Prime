// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "WormBoss.generated.h"

class ASword;
class UAttackData;
class AIndicatorDecal;

UCLASS()
class LIBERTYPRIME_API AWormBoss : public AMonster
{
	GENERATED_BODY()
	
public:

	AWormBoss();

	bool bJumpAttack;

	ALibertyPrimeCharacter* Target;
	FVector Jump_Loc;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Breathe;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		bool IsJumpStart;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		bool IsJumpEnd;

	UFUNCTION(BlueprintNativeEvent, Category = "CppFunctions")
		void Run_EQS();

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<AIndicatorDecal> IndicatorClass;

	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* Dissappear_Montage;
	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* Appear_Montage;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* Appear_SoundCue;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* Disappear_SoundCue;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Attack2;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Attack3;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Ground;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Ground_PS;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Idle_Loop;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> JawDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> BreatheDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		FVector TeleportLocation;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		float EQS_Radius;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* JawDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* BreatheDamager;

	FTimerHandle FlameTimer;
	FTimerHandle CrawlTimer;
	FTimerHandle JumpTimer;
	FTimerHandle WaitTimer;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Disappear();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Appear();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Disappear_Reset();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Disappear_Confirm();

	void Appear_Confirm();

	UPROPERTY()
		UAttackData* JumpAttackData;
	UPROPERTY()
		UAttackData* UndergroundAttackData;
	UPROPERTY()
		UAttackData* BreatheAttackData;

	void JumpAttack();
	void BreatheAttack();
	void UndergroundAttack();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void BreatheStart();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void BreatheEnd();
	void BreatheInit();
	void BreatheFunction();

	void ResetDamager_Add() override;
	void ResetMontage_Add() override;

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Crawl_Start();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Crawl_End();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Jump_Start();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Jump_End();

	void CrawlFunction();
	void JumpFunction();
	
};
