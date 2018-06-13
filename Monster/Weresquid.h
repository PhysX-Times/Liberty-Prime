// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "Weresquid.generated.h"

class UAttackData;
class ATrailMesh;
class AHomingProjectile;

UCLASS()
class LIBERTYPRIME_API AWeresquid : public AMonster
{
	GENERATED_BODY()
	
public:

	AWeresquid();

	int projectile_count;
	int current_yaw;

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

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Teleport_PS;
	UPROPERTY(Editanywhere, Category = "Particles")
		UParticleSystem* Projectile_PS;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Projectile;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Teleport;

	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* TeleportMontage;

	UPROPERTY()
		UAttackData* MagicAttackData;

	FTimerHandle MagicTimer;

	 void Magic_Teleport();

	 UFUNCTION(BlueprintNativeEvent, Category = "CppFunctions")
		 void Run_EQS();

	 UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		 void Teleport_Notify();
	 UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		 void Teleport_Apply();

	 void Magic_Staff();

	 UFUNCTION(BlueprintCallable, Category = "CppVariables")
		void Projectile_Notify();

	 void SpawnProjectile();

   	 virtual void BeginPlay() override;
	
};
