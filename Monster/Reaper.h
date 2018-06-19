// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "Reaper.generated.h"

class ASword;
class UAttackData;
class AArcherTrail;

UCLASS()
class LIBERTYPRIME_API AReaper : public AMonster
{
	GENERATED_BODY()
	
public:

	AReaper();

	ALibertyPrimeCharacter* Magic_Target;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Medium;

	TArray<FRotator> Earth_Rots;
	TArray<FVector> Earth_Locs;
	TArray<bool> Earth_Active;

	TArray<AArcherTrail*> Archers;

	float earth_distance;

	int archer_count;

	FRotator archer_init_rot;

	UPROPERTY()
		TArray<UParticleSystemComponent*> Earth_PSComps;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float Earth_Rot_Speed;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float Earth_Loc_Speed;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float Earth_Delay;

	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* MagicAttackA_Montage;
	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* MagicAttackB_Montage;

	UPROPERTY(EditAnywhere, Category = "Sounds")
		USoundCue* MagicB_Cue;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Archer;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Earth_Follow;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Earth_Explode;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Earth_Impact;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Earth_Follow_PS;
	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Earth_Explosion_PS;
	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Earth_Impact_PS;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> ScytheDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<AArcherTrail> ArcherTrailClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* ScytheDamager;

	UPROPERTY()
		UAttackData* MagicAttackData_A;
	UPROPERTY()
		UAttackData* MagicAttackData_B;
	UPROPERTY()
		UAttackData* ArcherAttackData;

	UPROPERTY()
		TArray<UAudioComponent*> Earth_Audio_Comps;

	void MagicAttack_A();
	void MagicAttack_B();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Earth_Notify();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Archer_Notify();

	void ArcherFunction();

	void Earth_Function();

	FTimerHandle EarthTimer;
	FTimerHandle ArcherTimer;

	virtual void BeginPlay() override;

};
