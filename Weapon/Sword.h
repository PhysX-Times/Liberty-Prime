// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "LibertyPrimeCharacter.h"
#include "Sword.generated.h"

UCLASS()
class LIBERTYPRIME_API ASword : public AWeapon
{
	GENERATED_BODY()

public:

	ASword();

	EDamageType DMGType;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		EWeaponType weapon_type;

	UPROPERTY()
	UParticleSystemComponent* ElementalPSComp;
	UPROPERTY()
	UParticleSystemComponent* WeaponTrailPSComp;

	UPROPERTY(EditAnywhere, Category = "Particle Effects")
		UParticleSystem* FirePS;
	UPROPERTY(EditAnywhere, Category = "Particle Effects")
		UParticleSystem* IcePS;
	UPROPERTY(EditAnywhere, Category = "Particle Effects")
		UParticleSystem* PoisonPS;
	UPROPERTY(EditAnywhere, Category = "Particle Effects")
		UParticleSystem* LightningPS;

	UPROPERTY(EditAnywhere, Category = "Particle Effects")
		UParticleSystem* FireTrailPS;
	UPROPERTY(EditAnywhere, Category = "Particle Effects")
		UParticleSystem* IceTrailPS;
	UPROPERTY(EditAnywhere, Category = "Particle Effects")
		UParticleSystem* PoisonTrailPS;
	UPROPERTY(EditAnywhere, Category = "Particle Effects")
		UParticleSystem* LightningTrailPS;
	UPROPERTY(EditAnywhere, Category = "Particle Effects")
		UParticleSystem* DefaultTrailPS;

	TArray<ALibertyPrimeCharacter*> DamagedChars;

	void ApplyEffect(EDamageType ElementalType, bool bSpawnSound = false);
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void ResetDamagedChars();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		virtual void StartTrail();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		virtual void EndTrail();

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Fire;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Ice;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Poison;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Lightning;

	UPROPERTY()
		UAudioComponent* SoundComponent_Weapon;

	void Clear_PS() override;

	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class UArrowComponent* GetElementalPSComp() { return ElementalPSPoint; }
	FORCEINLINE class UArrowComponent* GetWeaponTrailPSPoint() { return WeaponTrailPSPoint; }
	FORCEINLINE class UBoxComponent* GetSenser() { return Senser; }
	FORCEINLINE class USkeletalMeshComponent* GetWeapon_SK() { return Weapon_SK; }
	FORCEINLINE class UStaticMeshComponent* GetWeapon_ST() { return Weapon_ST; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RootScene, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* RootScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Senser, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* Senser;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WeaponTrailComp, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* WeaponTrailPSPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ElementalPoint, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* ElementalPSPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon_SK, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Weapon_SK;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon_ST, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Weapon_ST;
};
