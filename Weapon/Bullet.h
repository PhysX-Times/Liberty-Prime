// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "LibertyPrimeCharacter.h"
#include "Bullet.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API ABullet : public AWeapon
{
	GENERATED_BODY()

public:

	ABullet();

	EDamageType DMGType;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		EWeaponType weapon_type;

	TArray<ALibertyPrimeCharacter*> DamagedChars;

	UPROPERTY()
		UAttackData* AttackData;

	virtual void Tick(float DeltaTime) override;
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RootScene, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* RootScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Senser, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* Senser;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Head, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;
};
