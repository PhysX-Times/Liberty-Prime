// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "MasterGrunt.generated.h"

class ASword;
class UAttackData;
class AProjectile;

UCLASS()
class LIBERTYPRIME_API AMasterGrunt : public AMonster
{
	GENERATED_BODY()
	
public:

	AMasterGrunt();

	ALibertyPrimeCharacter* Cannon_Target;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Cannon_Max;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Cannon_Min;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Heal_Min;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> HandDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Impact_PS;
	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* CannonMuzzle_PS;
	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* AxeMuzzle_PS;
	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Heal_PS;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* RightHandDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* LeftHandDamager;

	UPROPERTY()
		UAttackData* CannonAttackData;
	UPROPERTY()
		UAttackData* HealAttackData;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Cannon;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Ground;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Impact;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Axe;

	FTimerHandle ImpactTimer;

	void CannonAttack();
	void Cast_Heal();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Heal_Notify();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Cannon_Fire();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Impact_Start();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Impact_End();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Impact_Right_Sound();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Impact_Left_Sound();

	void Impact_Ground(FVector TraceLoc, bool bMassDamage);
	void ImpactFunction();

	void ResetDamager_Add() override;

	virtual void BeginPlay() override;
	
};
