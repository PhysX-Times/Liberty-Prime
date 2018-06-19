// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "LibertyPrimeCharacter.h"
#include "BaseProjectile.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API ABaseProjectile : public AWeapon
{
	GENERATED_BODY()
	
public:

	ABaseProjectile();

	bool bArrow;
	bool bStopCheck;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		bool bExplosive;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		bool bDestroy_OnHit;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float Explosive_radius;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float Destroy_Delay;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		FVector PS_Scale;

	EDamageType DMGType;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		EWeaponType weapon_type;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Hit_PS;
	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Trail_PS;

	UPROPERTY(EditAnywhere, Category = "SoundCue")
		USoundCue* Hit_Cue;

	TArray<ALibertyPrimeCharacter*> DamagedChars;

	FTimerHandle DestroyTimer;

	UPROPERTY()
		UParticleSystemComponent* PS_TrailComp;
	UPROPERTY()
		UAttackData* AttackData;

	void DestroyFunction();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class USceneComponent* GetRootScene() { return RootScene; }

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RootScene, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* RootScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Senser, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* Senser;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PS_Arrow, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* PS_Arrow;
	
	
};
