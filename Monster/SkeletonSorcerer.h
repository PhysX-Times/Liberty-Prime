// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "SkeletonSorcerer.generated.h"

class ASword;
class UAttackData;
class ATrailMesh;
class AHomingProjectile;
class ASummonCaster;

UCLASS()
class LIBERTYPRIME_API ASkeletonSorcerer : public AMonster
{
	GENERATED_BODY()
	
public:

	ASkeletonSorcerer();

	int arrow_count_up;
	int arrow_count_down;

	int projectile_count;
	int current_yaw;

	ALibertyPrimeCharacter* Magic_Target;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Magic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CppVariables")
		float EQS_Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CppVariables")
		float Chase_Speed;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		FVector TeleportLocation;

	FVector arrow_current_loc;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ATrailMesh> TrailMeshClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> StaffDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<AHomingProjectile> ArrowProjectileClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<AHomingProjectile> HomingProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Teleport_PS;
	UPROPERTY(Editanywhere, Category = "Particles")
		UParticleSystem* Sigil_PS;
	UPROPERTY(Editanywhere, Category = "Particles")
		UParticleSystem* Sigil_End_PS;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Projectile;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Teleport;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* StaffDamager;

	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* TeleportMontage;

	UPROPERTY()
		UAttackData* MagicAttackData;
	UPROPERTY()
		UAttackData* ArrowAttackData;

	FTimerHandle MagicTimer;
	FTimerHandle ArrowUpTimer;
	FTimerHandle ArrowDownTimer;
	FTimerHandle ArrowTimer;

	UPROPERTY()
		UParticleSystemComponent* SigilPS_Comp;

	void Magic_Staff();
	void Magic_Teleport();
	void Magic_Arrow();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Projectile_Notify();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Arrow_Notify();

	UFUNCTION(BlueprintNativeEvent, Category = "CppFunctions")
		void Run_EQS();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Teleport_Notify();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Teleport_Apply();

	void SpawnProjectile();
	void SpawnDownArrow();
	void SpawnUpArrow();

	void Sigil_Destory();
	
	void ResetDamager_Add() override;
	void Die_Add() override;

	virtual void BeginPlay() override;

};
