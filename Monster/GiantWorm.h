// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "GiantWorm.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API AGiantWorm : public AMonster
{
	GENERATED_BODY()
	
public:

	AGiantWorm();

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Circle;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Random;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		TArray<FName> SocketNames_Ground;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Ground_PS;

	UPROPERTY(EditAnywhere, Category = "Materials")
		TArray<UMaterialInstance*> Mats;

	UFUNCTION(BlueprintNativeEvent, Category = "CppFunctions")
		void Run_EQS();

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
	FTimerHandle GroundTimer;
	FTimerHandle CrawlTimer;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Disappear();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Appear();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Disappear_Reset();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Disappear_Confirm();

	UPROPERTY()
		UAttackData* CircleAttackData;
	UPROPERTY()
		UAttackData* BreatheAttackData;

	void CircleAttack();
	void BreatheAttack();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
	void BreatheStart();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
	void BreatheEnd();
	void BreatheInit();
	void BreatheFunction();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Breathe_Check();

	void ResetDamager_Add() override;
	void ResetMontage_Add() override;
	
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Ground_Start();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Ground_End();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
	void Crawl_Start();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
	void Crawl_End();

	void GroundFunction();
	void CrawlFunction();
};
