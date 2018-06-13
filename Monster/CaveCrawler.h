// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "CaveCrawler.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API ACaveCrawler : public AMonster
{
	GENERATED_BODY()
	
public:

	ACaveCrawler(); 

	FTimerHandle GasTimer;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
	float AttackDistance_Gas;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> HandDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> GasDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> StingDamagerClass;

	UPROPERTY(EditAnywhere, Category = "Materials")
		TArray<UMaterialInstance*> CaveCrawlerMats;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* RightHandDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* LeftHandDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* StingDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* GasDamager;

	UPROPERTY()
		UAttackData* GasAttack;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Exhale;

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
