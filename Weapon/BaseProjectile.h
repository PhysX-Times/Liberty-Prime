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

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		bool bExplosive;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		bool bDestroy_OnHit;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float Explosive_radius;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		FVector PS_Scale;

	EDamageType DMGType;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		EWeaponType weapon_type;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Hit_PS;

	UPROPERTY(EditAnywhere, Category = "SoundCue")
		USoundCue* Hit_Cue;

	TArray<ALibertyPrimeCharacter*> DamagedChars;

	UPROPERTY()
		UAttackData* AttackData;

	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RootScene, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* RootScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Senser, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* Senser;
	
	
};
