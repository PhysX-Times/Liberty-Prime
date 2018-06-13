// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "BloodDecal.generated.h"

UCLASS()
class LIBERTYPRIME_API ABloodDecal : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RootScene, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* RootScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = RootScene, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* BloodDecal;
	
public:	

	ABloodDecal();

	UPROPERTY()
	UCurveFloat* CurveFloat;

	UPROPERTY(EditAnywhere, Category = "BloodMaterials")
	TArray<UMaterialInstance*> BloodMats;
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicInstance;

	FLinearColor BloodColor;

	FTimeline BloodLine;

	FTimerHandle BloodTimer;

	void ReverseDecal();
	void DestroySelf();
	UFUNCTION()
		void BloodFunction(float Val);

	virtual void Tick(float DeltaTime) override;


protected:

	virtual void BeginPlay() override;
	
};
