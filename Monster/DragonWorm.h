// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "DragonWorm.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API ADragonWorm : public AMonster
{
	GENERATED_BODY()
	
public:

	ADragonWorm();

	UFUNCTION(BlueprintNativeEvent, Category = "CppFunctions")
		void Run_EQS();

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Ground;

	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* Dissappear_Montage;
	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* Appear_Montage;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Ground_PS;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* Appear_SoundCue;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* Disappear_SoundCue;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> JawDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		FVector TeleportLocation;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		float EQS_Radius;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* JawDamager;

	FTimerHandle CrawlTimer;

	void ResetDamager_Add() override;

	virtual void BeginPlay() override;

	void Disappear();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
	void Appear();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
	void Disappear_Reset();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
	void Disappear_Confirm();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Crawl_Start();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Crawl_End();

	void CrawlFunction();
	
};
