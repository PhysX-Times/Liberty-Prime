// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "EarthTitan.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API AEarthTitan : public AMonster
{
	GENERATED_BODY()
	
public:

	AEarthTitan();

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Min;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Ground_PS;
	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Smash_PS;
	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Stomp_PS;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Ground;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Smash;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Stomp;

	FTimerHandle CrawlTimer_L;
	FTimerHandle CrawlTimer_R;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> ClawDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* RightClawDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* LeftClawDamager;

	UPROPERTY()
		UAttackData* ComboAttackData;
	UPROPERTY()
		UAttackData* SmashAttackData;

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
	void CrawlStart_R();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
	void CrawlEnd_R();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
	void CrawlStart_L();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
	void CrawlEnd_L();

	void CrawlFunction_R();
	void CrawlFunction_L();

	void CrawlFunction(FName SocketName);

	void SmashAttack();
	void ComboAttack();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Smash();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Stomp(FName SocketName);

	void Blast(FName SocketName, float radius, UAttackData* Target_Data, UParticleSystem* Target_PS, FVector Target_Scale, USoundCue* Target_Cue);

	void ResetDamager_Add() override;

	virtual void BeginPlay() override;
	
};
