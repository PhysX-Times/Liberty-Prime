// Fill out your copyright notice in the Description page of Project Settings.

#include "IndicatorDecal.h"
#include "Components/DecalComponent.h"
#include "TimerManager.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"


// Sets default values
AIndicatorDecal::AIndicatorDecal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
	IndicatorDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("IndicatorDecal"));
	IndicatorDecal->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AIndicatorDecal::BeginPlay()
{
	Super::BeginPlay();

	DynamicInstance = UMaterialInstanceDynamic::Create(IndicatorDecal->GetDecalMaterial(), this);
	IndicatorDecal->SetDecalMaterial(DynamicInstance);

	FOnTimelineFloat TLProgress;
	TLProgress.BindUFunction(this, FName("DecalFunction"));
	IndicatorLine.AddInterpFloat(CurveFloat, TLProgress);
	IndicatorLine.SetLooping(false);

	IndicatorLine.Play();

	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AIndicatorDecal::DestroyFunction, Duration, false);
}

void AIndicatorDecal::DecalFunction(float Val)
{
	DynamicInstance->SetScalarParameterValue(FName("Intensity"), Val);
}

// Called every frame
void AIndicatorDecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IndicatorLine.IsPlaying())
	{
		IndicatorLine.TickTimeline(DeltaTime);
	}

	AddActorWorldRotation(FQuat(FRotator(0.0f, RotateSpeed, 0.0f)));
}

void AIndicatorDecal::DestroyFunction()
{
	Destroy();
}