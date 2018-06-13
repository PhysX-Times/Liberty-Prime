// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "DarknessWarlord.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API ADarknessWarlord : public AMonster
{
	GENERATED_BODY()
	
public:

	ADarknessWarlord();
	
	UPROPERTY(EditAnywhere, Category = "SubClasses")
	TSubclassOf<ASword> ClawDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
	TSubclassOf<ASword> WeaponDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* ClawDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* WeaponDamager;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* WeaponFire_PS;

	UPROPERTY()
		UAudioComponent* WeaponCue;

	void Die_Add() override;
	virtual void BeginPlay() override;
};
