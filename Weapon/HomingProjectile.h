// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseProjectile.h"
#include "HomingProjectile.generated.h"

/**
 * 
 */
UCLASS()
class LIBERTYPRIME_API AHomingProjectile : public ABaseProjectile
{
	GENERATED_BODY()
	
public:

	AHomingProjectile();

	ALibertyPrimeCharacter* HomingTarget;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float HomingSpeed;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float HomingSpeed_Rot;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float Distance_Rot;

	bool bCanRot;
	
	virtual void Tick(float DeltaTime) override;
};
