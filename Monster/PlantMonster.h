// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "PlantMonster.generated.h"

class ASword;
class UAttackData;
class AHomingProjectile;

USTRUCT()
struct FPMMCombo
{
	GENERATED_BODY()

	FPMMCombo()
	{

	}

	UPROPERTY(EditAnywhere, Category = "Material")
		UMaterialInstance* MainMat;
	UPROPERTY(EditAnywhere, Category = "Material")
		UMaterialInstance* VinesMat;
	UPROPERTY(EditAnywhere, Category = "Material")
		UMaterialInstance* ThornsMat;
	UPROPERTY(EditAnywhere, Category = "Material")
		UMaterialInstance* FlowersMat;
};

UCLASS()
class LIBERTYPRIME_API APlantMonster : public AMonster
{
	GENERATED_BODY()
	
public:

	APlantMonster();

	int projectile_count;
	int current_yaw;

	ALibertyPrimeCharacter* HomingTarget;

	UPROPERTY(EditAnywhere, Category = "Materials")
		TArray<FPMMCombo> PlantMonster_Mats;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Magic_Max;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Magic_Min;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> JawDamagerClass;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<AHomingProjectile> HomingProjectileClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* JawDamager;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Attack1;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Charge;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Projectile;

	UPROPERTY(Editanywhere, Category = "Particles")
		UParticleSystem* Projectile_PS;

	FTimerHandle MagicTimer;

	UPROPERTY()
		UAttackData* MagicAttackData;

	void MagicAttack();

	void Spawn_Projectile_Function();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Spawn_HomingProjectiles();

	virtual void BeginPlay() override;
	
};
