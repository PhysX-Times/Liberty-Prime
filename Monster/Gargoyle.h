// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "Gargoyle.generated.h"

class ASword;
class UAttackData;
class AIndicatorDecal;
class AProjectile;

UCLASS()
class LIBERTYPRIME_API AGargoyle : public AMonster
{
	GENERATED_BODY()
	
public:

	AGargoyle();

	int meteor_count;
	int meteorCast_count;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		bool bIsFlyMode;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float FlyDistance;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float LandDistance_Max;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float LandDistance_Min;

	float WalkSpeed_Cahe;
	float RunSpeed_Cache;

	TArray<FTransform> IndicatorTrans;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<AIndicatorDecal> IndicatorClass;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Montages")
		FMontageData DeathFly_Data;
	UPROPERTY(EditAnywhere, Category = "Montages")
		FMontageData DeathStand_Data;
	UPROPERTY(EditAnywhere, Category = "Montages")
		FMontageData GetHitFly_Data;
	UPROPERTY(EditAnywhere, Category = "Montages")
		FMontageData GetHitStand_Data;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* FlySigil_PS;
	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* SigilDestroy_PS;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* StandSigil_PS;
	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* StandSigilDestroy_PS;
	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* StandOrb_PS;

	UPROPERTY()
		UParticleSystemComponent* FlySigil_PS_Comp;
	UPROPERTY()
		UParticleSystemComponent* StandSigil_PS_Comp;
	UPROPERTY()
		UParticleSystemComponent* Orb_PS_Comp;

	TArray<FTimerHandle> MeteorTimers;
	FTimerHandle MeteorCastTimer;
	FTimerHandle FlameTimer;

	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* Fly_Montage;
	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* Land_Montage;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> ClawDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> WingDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> TalonDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> FlameDamagerClass;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_AttackWing;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Wing;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_FlyCast;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_FlameAttack;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Orb;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* RightClawDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* LeftClawDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* RightWingDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* LeftWingDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* RightTalonDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* LeftTalonDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* FlameDamager;

	AIndicatorDecal* Cast_Indicator;

	UPROPERTY()
		UAttackData* JumpAttackData;

	UPROPERTY()
		UAttackData* FlyCastData;
	UPROPERTY()
		UAttackData* StandCastData;
	UPROPERTY()
		UAttackData* FlameAttackData;

	UPROPERTY()
		TArray<UAttackData*> RandomAttacks_FlyMode;

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void FlameStart();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void FlameEnd();
	void FlameInit();
	void FlameFunction();

	void FlyFunc();
	void LandFunc();

	void FlyMode_Cast();
	void StandMode_Cast();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Meteor_Start();

	void Meteor_Cast();
	void MeteorFunction();
	void FlameAttack();


	void RandomAttack() override;
	void ResetMontage_Add() override;
	void ResetDamager_Add() override;

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void OrbSpawn();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Sigil_Destroy();

	void StandSigil_Destroy();
	void Orb_Destroy();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
