// Fill out your copyright notice in the Description page of Project Settings.

#include "Flicker.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Components/PointLightComponent.h"


// Sets default values
AFlicker::AFlicker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
	FlickerLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("FlickerLight"));
	FlickerLight->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFlicker::BeginPlay()
{
	Super::BeginPlay();
	
	TargetIntensity = FlickerMax;
}

// Called every frame
void AFlicker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetIntensity == FlickerLight->Intensity)
	{
		TargetIntensity = UKismetMathLibrary::RandomFloatInRange(FlickerMin, FlickerMax);
	}

	float Delta_Int = TargetIntensity - FlickerLight->Intensity;
	float Target_Int = 0.0f;

	if (Delta_Int >= 0.0f)
	{
		Target_Int = UKismetMathLibrary::FClamp(FlickerLight->Intensity + FlickerSpeed, FlickerMin, TargetIntensity);
	}
	else
	{
		Target_Int = UKismetMathLibrary::FClamp(FlickerLight->Intensity - FlickerSpeed, TargetIntensity, FlickerMax);
	}

	FlickerLight->SetIntensity(Target_Int);
}

