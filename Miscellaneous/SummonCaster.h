// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "LibertyPrimeCharacter.h"
#include "GameFramework/Pawn.h"
#include "SummonCaster.generated.h"

UCLASS()
class LIBERTYPRIME_API ASummonCaster : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASummonCaster();

	float Opacity_Current;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float Opacity_Speed;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float Explosion_Radius;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		FRotator PS_Rot;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		FVector PS_Scale;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		FVector PS_Offset;

	ALibertyPrimeCharacter* MyOwner;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		EDamageType DMGType;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		EWeaponType WeaponType;

	UPROPERTY()
		UAttackData* AttackData;

	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* Cast_Montage;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Explosion_PS;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Explosion;

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Cast_Notify();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RootScene, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* RootScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Mesh;
	
};
