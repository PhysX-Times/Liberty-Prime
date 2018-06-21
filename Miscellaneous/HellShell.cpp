// Fill out your copyright notice in the Description page of Project Settings.

#include "HellShell.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "DrawDebugHelpers.h"

// Sets default values
AHellShell::AHellShell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	Shell = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shell"));
	Shell->SetupAttachment(RootComponent);

	Shell->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Shell->bGenerateOverlapEvents = false;
}

// Called when the game starts or when spawned
void AHellShell::BeginPlay()
{
	Super::BeginPlay();
	
	FOnTimelineFloat ProgressFunction;
	ProgressFunction.BindUFunction(this, FName("Shell_Function"));

	Opacityline.AddInterpFloat(OpacityCurve, ProgressFunction);
	Opacityline.SetLooping(false);

	GetWorldTimerManager().ClearTimer(InitTimer);
	GetWorldTimerManager().SetTimer(InitTimer, this, &AHellShell::Hell_Notify, Opacityline.GetTimelineLength(), false);

	Opacityline.PlayFromStart();
}

// Called every frame
void AHellShell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Opacityline.IsPlaying())
	{
		Opacityline.TickTimeline(DeltaTime);
	}
}

void AHellShell::Hell_Notify()
{
	bCanTrace = true;

	for (auto HellFire_Comp : HellFire_Comps)
	{
		if (HellFire_Comp)
		{
			HellFire_Comp->Activate();
		}
	}

	SoundComp_Fire = MyOwner->Play_SoundCue_Location(SoundCue_Fire, GetActorLocation());

	Hell_Function();
	GetWorldTimerManager().SetTimer(TraceTimer, this, &AHellShell::Trace_Reset, 5.0f, false);
}

void AHellShell::Hell_Function()
{
	if (bCanTrace)
	{
		if (!MyOwner->IsDead)
		{
			MyOwner->MassDamage(TraceRadius, HellAttackData, GetActorLocation(), EWeaponType::Weapon_None, this, true, EDamageType::Damage_Fire);
			//DrawDebugSphere(GetWorld(), GetActorLocation(), TraceRadius, 16, FColor::Red, false, 0.5f, 0, 5.5f);

			GetWorldTimerManager().ClearTimer(HellTimer);
			GetWorldTimerManager().SetTimer(HellTimer, this, &AHellShell::Hell_Function, 0.1f, false);
		}
		else
		{
			GetWorldTimerManager().ClearTimer(TraceTimer);
			Trace_Reset();
		}
	}
}

void AHellShell::Trace_Reset()
{
	MyOwner->SoundComponents.Add(SoundComp_Fire);
	bCanTrace = false;

	for (auto HellFire_Comp : HellFire_Comps)
	{
		if (HellFire_Comp)
		{
			HellFire_Comp->Deactivate();
		}
	}

	GetWorldTimerManager().ClearTimer(InitTimer);
	GetWorldTimerManager().SetTimer(InitTimer, this, &AHellShell::DestroyFunction, Opacityline.GetTimelineLength(), false);

	Opacityline.ReverseFromEnd();
}

void AHellShell::Shell_Function(float Value)
{
	for (int i = 0; i < Shell->GetNumMaterials(); i++)
	{
		UMaterialInstanceDynamic* DynamicInstance = Shell->CreateAndSetMaterialInstanceDynamic(i);
		DynamicInstance->SetScalarParameterValue(FName("Opacity"), Value);
	}
}

void AHellShell::DestroyFunction()
{
	Destroy();
}