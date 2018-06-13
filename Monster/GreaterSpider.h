// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "GreaterSpider.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API AGreaterSpider : public AMonster
{
	GENERATED_BODY()
	
public:

	AGreaterSpider();

	UPROPERTY(EditAnywhere, Category = "Materials")
		TArray<UMaterialInstance*> Mesh1_Mats;
	UPROPERTY(EditAnywhere, Category = "Materials")
		TArray<UMaterialInstance*> Mesh2_Mats;

	UPROPERTY(EditAnywhere, Category = "SkeletalMeshes")
		USkeletalMesh* Mesh_1;
	UPROPERTY(EditAnywhere, Category = "SkeletalMeshes")
		USkeletalMesh* Mesh_2;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Charge_Max;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Charge_Min;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Ground;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Impact;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Ground_PS;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> ClawDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* RightClawDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* LeftClawDamager;

	UPROPERTY()
		UAttackData* ChargeAttackData;

	FTimerHandle CrawlTimer;

	void ChargeAttack();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Crawl_Start();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Crawl_End();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Impact_Claw();

	void CrawlFunction();
	void ImpactFunction(FVector TraceLoc, bool bImpact);

	void ResetDamager_Add() override;

	virtual void BeginPlay() override;
	
};
