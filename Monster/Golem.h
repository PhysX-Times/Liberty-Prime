// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "Golem.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API AGolem : public AMonster
{
	GENERATED_BODY()
	
public:

	AGolem();

	UPROPERTY(EditAnywhere, Category = "Materials")
		TArray<UMaterialInstance*> RockMats;

	UPROPERTY(EditAnywhere, Category = "Particles")
		UParticleSystem* SmashPS;

	UPROPERTY(EditAnywhere, Category = "Sounds")
		USoundCue* SmashCue;

	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* Pose_Montage;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_GetPose;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> HandDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* RightHandDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* LeftHandDamager;

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Smash();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Pose_End();
	UFUNCTION(BlueprintNativeEvent, Category = "CppFunctions")
		void Pose_Notify();

	void Build_Up() override;

	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ConstructMesh, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* ConstructMesh;
	
};
