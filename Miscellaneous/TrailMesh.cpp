// Fill out your copyright notice in the Description page of Project Settings.

#include "TrailMesh.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"

// Sets default values
ATrailMesh::ATrailMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(FName("RootScene"));
	RootComponent = RootScene;
	Static_TrailMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Static_TrailMesh"));
	Static_TrailMesh->SetupAttachment(RootComponent);

	Opacity_Current = 1.0f;
}

// Called every frame
void ATrailMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Opacity_Current -= OpacitySpeed;

	if (Opacity_Current <= 0.0f)
	{
		Destroy();
	}

	for (int i = 0; i < Static_TrailMesh->GetNumMaterials(); i++)
	{
		UMaterialInstanceDynamic* DynamicInstance = Static_TrailMesh->CreateAndSetMaterialInstanceDynamic(i);
		DynamicInstance->SetScalarParameterValue(FName("Opacity"), Opacity_Current);
	}
}

