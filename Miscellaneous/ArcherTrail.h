// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "LibertyPrimeCharacter.h"
#include "GameFramework/Pawn.h"
#include "ArcherTrail.generated.h"

class AHomingProjectile;

UCLASS()
class LIBERTYPRIME_API AArcherTrail : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArcherTrail();

	int charge_count;

	bool can_rot;

	ALibertyPrimeCharacter* MyOwner;

	UAttackData* ArrowAttackData;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		FVector Rot_Offset;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float Rot_Speed;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Arrow;

	UPROPERTY()
		TArray<UMeshComponent*> Opacity_Meshes;

	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* OpacityCurve;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Charge_PS;
	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* Charge_End_PS;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<AHomingProjectile> ArrowProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* Mesh_Charge_Montage;
	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* Bow_Charge_Montage;

	FTimerHandle ChargeTimer;

	FTimeline Opacityline;

	UPROPERTY()
		UParticleSystemComponent* Charge_PSComp;

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Charge_Notify();

	UFUNCTION()
		void OpacityFunction(float Value);

	void Charge_Function();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RootScene, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* RootScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Bow, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Bow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Quiver, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Quiver;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Charge_Trans_Comp, meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* Charge_Trans_Comp;
	
};
