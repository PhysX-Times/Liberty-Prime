// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "BoneDragon_Twin.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API ABoneDragon_Twin : public AMonster
{
	GENERATED_BODY()
	
public:

	ABoneDragon_Twin();

	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* Pose_Montage;
	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* Rubble_Idle_Montage;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> TailDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> BiteDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* TailDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* BiteDamager;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_GetPose;

	UPROPERTY()
		UAttackData* BiteAttackData;
	UPROPERTY()
		UAttackData* TailLeftAttackData;
	UPROPERTY()
		UAttackData* TailRightAttackData;

	void Attack_Bite();
	void Attack_Tail_Left();
	void Attack_Tail_Right();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Pose_End();
	UFUNCTION(BlueprintNativeEvent, Category = "CppFunctions")
		void Pose_Notify();

	void Build_Up() override;

	virtual void BeginPlay() override;
	
};
