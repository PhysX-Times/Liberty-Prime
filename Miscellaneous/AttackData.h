// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMontage.h"
#include "UObject/NoExportTypes.h"
#include "Animation/AnimMontage.h"
#include "LibertyPrimeCharacter.h"
#include "AttackData.generated.h"

UCLASS()
class LIBERTYPRIME_API UAttackData : public UObject
{
	GENERATED_BODY()

public:

	UAttackData();

	ERestriction restriction;
	EWeaponType WeaponType;

	float DMGMultiplier;
	float RotSpeed;
	float PlaySpeed;

	EImpact impact;

	bool IsSequencer;

	UPROPERTY()
	UAnimMontage* AttackMontage;
	UPROPERTY()
	TArray<UAnimMontage*> AttackMontages;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
	USoundCue* AttackSoundCue;

};
