// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "FatOgre.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API AFatOgre : public AMonster
{
	GENERATED_BODY()
	
public:

	AFatOgre();

	UPROPERTY(EditAnywhere, Category = "CppVariables")
	float AttackDistance_Roar;
	
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> HandDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* HandDamager;

	UPROPERTY()
		UAttackData* RoarData;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* RoarPS;

	void Roar();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
	void RoarNotify();

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Roar;

	virtual void BeginPlay() override;
};
