// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "Weresquid.generated.h"

class UAttackData;
class ATrailMesh;
class AHomingProjectile;
class ASummonCaster;

UCLASS()
class LIBERTYPRIME_API AWeresquid : public AMonster
{
	GENERATED_BODY()
	
public:

	AWeresquid();

	int projectile_count;
	int current_yaw;

	int arrow_count;

	float summon_yaw;
	int summon_count;

	bool can_cast;

	ALibertyPrimeCharacter* Magic_Target;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		FVector TeleportLocation;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Staff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CppVariables")
		float EQS_Radius;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ATrailMesh> TrailMeshClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<AHomingProjectile> HomingProjectileClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<AHomingProjectile> ArrowProjectileClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASummonCaster> SummonCasterClass;

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
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Arrow;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Magic_Arrow;

	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* TeleportMontage;

	UPROPERTY()
		UAttackData* MagicAttackData;
	UPROPERTY()
		UAttackData* ArrowAttackData;
	UPROPERTY()
		UAttackData* SummonAttackData;

	UPROPERTY()
		UParticleSystemComponent* SigilPS_Comp;

	FTimerHandle MagicTimer;
	FTimerHandle ArrowTimer;
	FTimerHandle SummonTimer;
	FTimerHandle CastTimer;

	 void Magic_Teleport();

	 UFUNCTION(BlueprintNativeEvent, Category = "CppFunctions")
		 void Run_EQS();

	 UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		 void Teleport_Notify();
	 UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		 void Teleport_Apply();

	 void Magic_Staff();
	 void Magic_Arrow();
	 void Magic_Summon();

	 UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Projectile_Notify();

	 UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		 void Arrow_Notify();

	 UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		 void Summon_Notify();

	 void SummonFunction();

	 void SpawnProjectile();
	 void SpawnArrow();

	 void Sigil_Destory();

	 void Cast_Reset();
	 void Cast_Function(float cast_delay);

	 void ResetDamager_Add() override;
	 void Die_Add() override;

   	 virtual void BeginPlay() override;
	 virtual void Tick(float DeltaTime) override;
	
};
