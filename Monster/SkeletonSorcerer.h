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
class AIndicatorDecal;
class AHellShell;

UCLASS()
class LIBERTYPRIME_API ASkeletonSorcerer : public AMonster
{
	GENERATED_BODY()
	
public:

	ASkeletonSorcerer();

	bool can_cast;

	int arrow_count_up;
	int arrow_count_down;

	int projectile_count;

	int current_yaw;

	ALibertyPrimeCharacter* Magic_Target;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Magic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CppVariables")
		float EQS_Radius;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
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
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASummonCaster> SummonCasterClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<AHellShell> HellShellClass;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Teleport_PS;
	UPROPERTY(Editanywhere, Category = "Particles")
		UParticleSystem* Sigil_PS;
	UPROPERTY(Editanywhere, Category = "Particles")
		UParticleSystem* Sigil_End_PS;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Projectile;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Arrow;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Teleport;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_HellShell;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Melee;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* StaffDamager;

	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* TeleportMontage;

	UPROPERTY()
		UAttackData* MagicAttackData;
	UPROPERTY()
		UAttackData* ArrowAttackData;
	UPROPERTY()
		UAttackData* HellAttackData;
	UPROPERTY()
		UAttackData* TeleportAttackData;

	FTimerHandle MagicTimer;
	FTimerHandle ArrowUpTimer;
	FTimerHandle ArrowDownTimer;
	FTimerHandle ArrowTimer;
	FTimerHandle CastTimer;

	UPROPERTY()
		UParticleSystemComponent* SigilPS_Comp;

	void Magic_Staff();
	void Magic_Teleport();
	void Magic_Arrow();
	void Magic_Shell();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Projectile_Notify();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Arrow_Notify();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void HellShell_Notify();

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

	void Cast_Reset();
	void Cast_Function(float cast_delay);
	
	void ResetDamager_Add() override;
	void Die_Add() override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
