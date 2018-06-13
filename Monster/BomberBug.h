// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "BomberBug.generated.h"

class ABlow;
class ASword;
class UAttackData;

USTRUCT()
struct FMaterialCombo
{
	GENERATED_BODY()

	FMaterialCombo()
	{

	}

	FMaterialCombo(UMaterialInstance* BomberBugMat_Par, UMaterialInstance* BomberBugAddOnsMat_Par)
	{
		BomberBugMat = BomberBugMat_Par;
		BomberBugAddOnsMat = BomberBugAddOnsMat_Par;
	}

	UPROPERTY(EditAnywhere, Category = "Material")
		UMaterialInstance* BomberBugMat;
	UPROPERTY(EditAnywhere, Category = "Material")
		UMaterialInstance* BomberBugAddOnsMat;
};

UCLASS()
class LIBERTYPRIME_API ABomberBug : public AMonster
{
	GENERATED_BODY()
	
public:

	ABomberBug();

	UPROPERTY(EditAnywhere, Category = "Materials")
		TArray<FMaterialCombo> BomberBug_Mats;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Spit;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> JawDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ABlow> BlowDamagerClass;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Explode_PS;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Horn;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Spit;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Explode;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* JawDamager;

	UPROPERTY()
		UAttackData* SpitAttackData;

	void SpitAttack();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Spit_Notify();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Explode_Notify();

	void Die_Add() override;

	virtual void BeginPlay() override;
	
};
