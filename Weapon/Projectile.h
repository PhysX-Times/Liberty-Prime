// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseProjectile.h"
#include "LibertyPrimeCharacter.h"
#include "Projectile.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AProjectile : public ABaseProjectile
{
	GENERATED_BODY()
	
public:

	AProjectile();

	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() { return ProjectileMovement; }

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Head, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;
	
};
