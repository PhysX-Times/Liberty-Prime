// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "GiantTroll.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API AGiantTroll : public AMonster
{
	GENERATED_BODY()
	
	
public:

	AGiantTroll();

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Smash;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> HandDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* LeftHandDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* RightHandDamager;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* SmashPS;

	UPROPERTY(EditAnywhere, Category = "Sounds")
		USoundCue* SmashCue;

	UPROPERTY()
	UAttackData* SmashAttackData;

	void SmashAttack();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
	void SmashBlast();

	virtual void BeginPlay() override;
	
};
