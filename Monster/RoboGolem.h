// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "RoboGolem.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API ARoboGolem : public AMonster
{
	GENERATED_BODY()
	
public:

	ARoboGolem();

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
	bool bRolePose;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Medium;

	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* RollStart_Montage;
	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* RollEnd_Montage;

	UPROPERTY(EditAnywhere, Category = "Montages")
		UParticleSystem* HandSmashPS;
	UPROPERTY(EditAnywhere, Category = "Montages")
		UParticleSystem* LegSmashPS;

	UPROPERTY(EditAnywhere, Category = "Sounds")
		USoundCue* HandSmashCue;
	UPROPERTY(EditAnywhere, Category = "Sounds")
		USoundCue* LegSmashCue;
	UPROPERTY(EditAnywhere, Category = "Sounds")
		USoundCue* RollCue;


	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> HandDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* HandDamager;

	UPROPERTY()
		UAttackData* MagicAttackData_A;
	UPROPERTY()
		UAttackData* MagicAttackData_B;

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
	void HandSmash();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
	void LegSmash();

	virtual void BeginPlay() override;

	void Roll_Start();
	void Roll_End();

	void Smash(FName SocketName, float radius, FVector PS_Scale, UParticleSystem* SmashPS, USoundCue* SmashCue);
	
};
