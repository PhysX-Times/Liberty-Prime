// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "MountainDragon.generated.h"

class ASword;
class UAttackData;
class AHomingProjectile;

UCLASS()
class LIBERTYPRIME_API AMountainDragon : public AMonster
{
	GENERATED_BODY()

public:

	AMountainDragon();

	UPROPERTY(EditAnywhere, Category = "CppVaraibles")
		float Start_Chase_Distance;

	TArray<ALibertyPrimeCharacter*> DamagedChars;

	bool bFireBreathe;

	FRotator TargetRot;

	FTimerHandle FlameTimer;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> FlameDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<AHomingProjectile> HomingProjectileClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* FlameDamager;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* FlamePS_End;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_FireBreathe;

	UPROPERTY()
		UAttackData* FlameThrowAttack;
	UPROPERTY()
		UAttackData* FireBallAttack;

	UPROPERTY()
		UParticleSystemComponent* FirstPS_End_Comp;

	void FlameThrow();
	void FireBallThrow();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void FireBall_Notify();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void FlameStart();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void FlameEnd();
	void FlameInit();
	void FlameFunction();

	void ResetDamager_Add() override;
	void ResetMontage_Add() override;
	void PlayMontage_Add() override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
