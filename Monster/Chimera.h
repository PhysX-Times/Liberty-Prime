// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "Chimera.generated.h"

class ASword;
class UAttackData;
class AIndicatorDecal;

UCLASS()
class LIBERTYPRIME_API AChimera : public AMonster
{
	GENERATED_BODY()
	
public:

	AChimera();

	FTransform IndicatorTrans;

	FTimerHandle CastTimer;
	FTimerHandle SummonTimer;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
	float AttackDistance_Medium;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<AIndicatorDecal> IndicatorClass;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> HandDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> BiteDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> SnakeDamagerClass;

	UPROPERTY(EditAnywhere, Category = "SoundCue")
	USoundCue* SoundCue_Summon;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* RightHandDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* LeftHandDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* BiteDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* SnakeDamager;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* CastPS;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* GoatPS;

	UPROPERTY()
		UParticleSystemComponent* GoatPSComp;

	UPROPERTY()
		UAttackData* RamAttackData;
	UPROPERTY()
		UAttackData* CastData;

	void ChimeraCast();
	void Summon();

	void RamAttack();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
