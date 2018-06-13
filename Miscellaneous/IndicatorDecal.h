// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IndicatorDecal.generated.h"

UCLASS()
class LIBERTYPRIME_API AIndicatorDecal : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RootScene, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* RootScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RootScene, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* IndicatorDecal;
	
public:	
	// Sets default values for this actor's properties
	AIndicatorDecal();

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float RotateSpeed;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float Duration;

	FTimerHandle DestroyTimer;

	FTimeline IndicatorLine;

	UPROPERTY(EditAnywhere, Category = "TimelineCurve")
		UCurveFloat* CurveFloat;

	UPROPERTY()
		UMaterialInstanceDynamic* DynamicInstance;

	UFUNCTION()
		void DecalFunction(float Val);

	void DestroyFunction();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
