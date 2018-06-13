// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrailMesh.generated.h"

UCLASS()
class LIBERTYPRIME_API ATrailMesh : public AActor
{
	GENERATED_BODY()
	
public:	

	ATrailMesh();

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float OpacitySpeed;

	float Opacity_Current;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Static_TrailMesh, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* RootScene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Static_TrailMesh, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* Static_TrailMesh;
	
};
