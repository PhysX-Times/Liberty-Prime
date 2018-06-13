//// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "Troll.generated.h"

class ASword;
class ADecorator;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API ATroll : public AMonster
{
	GENERATED_BODY()

public:

	ATroll();

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Medium;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Long;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* RoarPS;
	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* OverHeadPS;

	UPROPERTY(EditAnywhere, Category = "Materials")
		TArray<UMaterialInstance*> TrollMats;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> RightHandDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> LeftHandDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ADecorator> HelmetClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* RightHandDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* LeftHandDamager;

	UPROPERTY()
		UAttackData* SwipeAttackData;
	UPROPERTY()
		UAttackData* HeadOverAttackData;
	UPROPERTY()
		UAttackData* DashAttackData;
	UPROPERTY()
		UAttackData* RoarAttackData;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Roar;

	void SwipeAttack();
	void HeadOverAttack();
	void DashAttack();
	void Roar();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void RoarNotify();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void OverHeadNotify();

	void ApplyMass(FVector StartLoc, FVector PS_Scale, UParticleSystem* TargetPS, float radius);

	virtual void BeginPlay() override;
};
