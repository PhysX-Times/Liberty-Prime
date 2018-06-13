// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "TreeEnt.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API ATreeEnt : public AMonster
{
	GENERATED_BODY()
	
public:

	ATreeEnt();

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Medium;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> HandDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* RightHandDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* LeftHandDamager;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		UParticleSystem* StompPS;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		UParticleSystem* RoarPS;

	UPROPERTY()
		UAttackData* StompData;
	UPROPERTY()
		UAttackData* RoarData;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Roar;

	void Roar();
	void Stomp();

	UFUNCTION(BlueprintCallable, Category = "CppVariables")
		void Stomp_Notify();
	UFUNCTION(BlueprintCallable, Category = "CppVariables")
		void Roar_Notify();

	void MassDamage_Local(float radius, UParticleSystem* TargetPS, FVector Scale, EWeaponType WeaponType);

	virtual void BeginPlay() override;
	
};
