// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "Bear.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API ABear : public AMonster
{
	GENERATED_BODY()
	
public:

	ABear();

	UPROPERTY(EditAnywhere, Category = "Materials")
		TArray<UMaterialInstance*> BearMats;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> HandDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* RightHandDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* LeftHandDamager;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Attack_Medium;

	virtual void BeginPlay() override;
	
};
