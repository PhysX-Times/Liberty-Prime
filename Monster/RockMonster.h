// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "RockMonster.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API ARockMonster : public AMonster
{
	GENERATED_BODY()
	
public:

	ARockMonster();

	bool bLaserActive;

	TArray<ALibertyPrimeCharacter*> DamagedChars;

	UPROPERTY(EditAnywhere, Category = "Materials")
	TArray<UMaterialInstance*> RockMats;

	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* Pose_Montage;
	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* Rubble_Idle_Montage;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Medium;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Attack1;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Attack_Laser;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_GetPose;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* LaserParticle;
	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* LaserParticle_End;

	UPROPERTY()
		UParticleSystemComponent* LaserPS_Comp;
	UPROPERTY()
		UParticleSystemComponent* LaserPS_End_Comp;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> HandDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* RightHandDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* LeftHandDamager;

	UPROPERTY()
		UAttackData* LaserAttackData;

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Pose_End();
	UFUNCTION(BlueprintNativeEvent, Category = "CppFunctions")
		void Pose_Notify();
	

	void LaserAttack();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Laser_Start();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Laser_End();

	void Build_Up() override;
	void ResetDamager_Add() override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
