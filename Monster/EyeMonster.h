// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "EyeMonster.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API AEyeMonster : public AMonster
{
	GENERATED_BODY()
	
public:

	AEyeMonster();

	FTimerHandle FlameTimer;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Spray_Max;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Spray_Min;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Sigil_Destroy_PS;
	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* PoisonSigil_PS;

	UPROPERTY()
		UParticleSystemComponent* PoisonSigil_Comp;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_BumpAttack;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_SprayAttack;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> HeadDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> TentacleDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> FlameDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* HeadDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* TentacleDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* FlameDamager;

	UPROPERTY()
		UAttackData* SprayAttackData;

	void SprayAttack();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void FlameStart();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void FlameEnd();
	void FlameInit();
	void FlameFunction();

	void ResetDamager_Add() override;

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Start_Movement();

	void Destroy_Sigil();

	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Head, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* SprayArrow;
	
};
