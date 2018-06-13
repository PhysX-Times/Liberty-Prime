// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Flicker.generated.h"

UCLASS()
class LIBERTYPRIME_API AFlicker : public AActor
{
	GENERATED_BODY()

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RootScene, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RootScene, meta = (AllowPrivateAccess = "true"))
	class UPointLightComponent* FlickerLight;
	
public:	
	// Sets default values for this actor's properties
	AFlicker();

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float FlickerSpeed;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float FlickerMax;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float FlickerMin;

	float TargetIntensity;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
