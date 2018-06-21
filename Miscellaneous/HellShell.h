// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LibertyPrimeCharacter.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Actor.h"
#include "HellShell.generated.h"

UCLASS()
class LIBERTYPRIME_API AHellShell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHellShell();
	
	bool bCanTrace;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float TraceRadius;

	UAttackData* HellAttackData;

	ALibertyPrimeCharacter* MyOwner;

	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* OpacityCurve;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Fire;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		TArray<UParticleSystemComponent*> HellFire_Comps;

	UPROPERTY()
		UAudioComponent* SoundComp_Fire;

	FTimeline Opacityline;

	FTimerHandle InitTimer;
	FTimerHandle HellTimer;
	FTimerHandle TraceTimer;

	void Hell_Notify();
	void Hell_Function();

	void Trace_Reset();

	void DestroyFunction();

	UFUNCTION()
		void Shell_Function(float Value);

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Senser, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* RootScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Senser, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Shell;
	
};
