// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "LibertyPrimeCharacter.h"
#include "Blow.generated.h"

UCLASS()
class LIBERTYPRIME_API ABlow : public AWeapon
{
	GENERATED_BODY()
	
public:

	ABlow();

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float MaxDistance;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float MaxDelay;

	float TimeElapsed;

	UPROPERTY(EditAnywhere, Category = "Particle Effects")
		UParticleSystem* BlowPS;

	EDamageType DMGType;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		EWeaponType weapon_type;

	TArray<ALibertyPrimeCharacter*> DamagedChars;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RootScene, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* RootScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Senser, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* Senser;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WeaponTrailComp, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* BlowPSPoint;
};
