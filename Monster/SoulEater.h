// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "SoulEater.generated.h"

class ASword;
class ABlow;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API ASoulEater : public AMonster
{
	GENERATED_BODY()
	
public:

	ASoulEater();

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Roar;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> ClawDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ABlow> BlowDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* ClawDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* BlowDamager;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* SmashPS;

	UPROPERTY(EditAnywhere, Category = "Sounds")
		USoundCue* SmashCue;

	UPROPERTY()
		UAttackData* SpitAttackData;

	void SpitVenom();

	UFUNCTION(BlueprintCallable, Category = "CppFunction")
		void Spit_notify();

	virtual void BeginPlay() override;
	
};
