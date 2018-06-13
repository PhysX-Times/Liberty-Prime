// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Knight/Knight.h"
#include "SnSKnight.generated.h"

class AProjectile;

UCLASS()
class LIBERTYPRIME_API ASnSKnight : public AKnight
{
	GENERATED_BODY()

public:

	ASnSKnight();

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Long_Min;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<class AProjectile> Projectile_Fire_Class;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<class AProjectile> Projectile_Ice_Class;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<class AProjectile> Projectile_Poison_Class;
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<class AProjectile> Projectile_Lightning_Class;

	TArray<AActor*> BlastActors;

	UPROPERTY(EditAnywhere, Category = "ParticleEffects")
		UParticleSystem* SpinFirePS;
	UPROPERTY(EditAnywhere, Category = "ParticleEffects")
		UParticleSystem* SpinIcePS;
	UPROPERTY(EditAnywhere, Category = "ParticleEffects")
		UParticleSystem* SpinPoisonPS;
	UPROPERTY(EditAnywhere, Category = "ParticleEffects")
		UParticleSystem* SpinLightningPS;

	UPROPERTY(EditAnywhere, Category = "ParticleEffects")
		UParticleSystem* ImpactLightningPS;
	UPROPERTY(EditAnywhere, Category = "ParticleEffects")
		UParticleSystem* ImpactFirePS;
	UPROPERTY(EditAnywhere, Category = "ParticleEffects")
		UParticleSystem* ImpactPoisonPS;
	UPROPERTY(EditAnywhere, Category = "ParticleEffects")
		UParticleSystem* ImpactIcePS;

	UPROPERTY(EditAnywhere, Category = "Sounds")
		USoundCue* SoundCue_ImpactFire;
	UPROPERTY(EditAnywhere, Category = "Sounds")
		USoundCue* SoundCue_ImpactIce;
	UPROPERTY(EditAnywhere, Category = "Sounds")
		USoundCue* SoundCue_ImpactPoison;
	UPROPERTY(EditAnywhere, Category = "Sounds")
		USoundCue* SoundCue_ImpactLightning;

	UPROPERTY(EditAnywhere, Category = "Sounds")
		USoundCue* SoundCue_SpinElemental;
	UPROPERTY(EditAnywhere, Category = "Sounds")
		USoundCue* SoundCue_Spin;

	UPROPERTY(EditAnywhere, Category = "Sounds")
		USoundCue* SoundCue_ImpactAttack;

	UParticleSystemComponent* SpinPS;

	UPROPERTY()
		UAttackData* DashAttackData;
	UPROPERTY()
		UAttackData* SpinAttackData;
	UPROPERTY()
		UAttackData* EnergyAttackData;
	UPROPERTY()
		UAttackData* ImpactAttackData;

	FTimerHandle SpinTimer;
	FTimerHandle ImpactTimer;

	FVector ImpactInit_Vector;
	FVector ImpactForward_Vector;

	int ImpactBlast_Count;

	bool IsSpinning;

	void DashAttack();
	void EnergyAttack();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
	void EnergySlash_Generate();
	void ImpactAttack();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
	void ImpactBlast();

	void SpinAttack();
	void SpinReset();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void SpinStart();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void SpinEnd();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WeaponTrailComp, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* SlashPoint;
};
