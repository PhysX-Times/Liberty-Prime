// Fill out your copyright notice in the Description page of Project Settings.

#include "BloodDecal.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "TimerManager.h"


// Sets default values
ABloodDecal::ABloodDecal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
	BloodDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("BloodDecal"));
	BloodDecal->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BloodMat1(TEXT("MaterialInstanceConstant'/Game/Effects/Blood_Decals/BD_Inst1.BD_Inst1'"));
	BloodMats.Add(BloodMat1.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BloodMat2(TEXT("MaterialInstanceConstant'/Game/Effects/Blood_Decals/BD_Inst2.BD_Inst2'"));
	BloodMats.Add(BloodMat2.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BloodMat3(TEXT("MaterialInstanceConstant'/Game/Effects/Blood_Decals/BD_Inst3.BD_Inst3'"));
	BloodMats.Add(BloodMat3.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BloodMat4(TEXT("MaterialInstanceConstant'/Game/Effects/Blood_Decals/BD_Inst4.BD_Inst4'"));
	BloodMats.Add(BloodMat4.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BloodMat5(TEXT("MaterialInstanceConstant'/Game/Effects/Blood_Decals/BD_Inst5.BD_Inst5'"));
	BloodMats.Add(BloodMat5.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BloodMat6(TEXT("MaterialInstanceConstant'/Game/Effects/Blood_Decals/BD_Inst6.BD_Inst6'"));
	BloodMats.Add(BloodMat6.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BloodMat7(TEXT("MaterialInstanceConstant'/Game/Effects/Blood_Decals/BD_Inst7.BD_Inst7'"));
	BloodMats.Add(BloodMat7.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BloodMat8(TEXT("MaterialInstanceConstant'/Game/Effects/Blood_Decals/BD_Inst8.BD_Inst8'"));
	BloodMats.Add(BloodMat8.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BloodMat9(TEXT("MaterialInstanceConstant'/Game/Effects/Blood_Decals/BD_Inst9.BD_Inst9'"));
	BloodMats.Add(BloodMat9.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BloodMat10(TEXT("MaterialInstanceConstant'/Game/Effects/Blood_Decals/BD_Inst10.BD_Inst10'"));
	BloodMats.Add(BloodMat10.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BloodMat11(TEXT("MaterialInstanceConstant'/Game/Effects/Blood_Decals/BD_Inst11.BD_Inst11'"));
	BloodMats.Add(BloodMat11.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BloodMat12(TEXT("MaterialInstanceConstant'/Game/Effects/Blood_Decals/BD_Inst12.BD_Inst12'"));
	BloodMats.Add(BloodMat12.Object);
}

void ABloodDecal::BeginPlay()
{
	Super::BeginPlay();

	int rand = UKismetMathLibrary::RandomIntegerInRange(0, BloodMats.Num() - 1);
	DynamicInstance = UMaterialInstanceDynamic::Create(BloodMats[rand], this);
	BloodDecal->SetDecalMaterial(DynamicInstance);

	DynamicInstance->SetVectorParameterValue(FName("Color"), BloodColor);

	FStringAssetReference CurveFloatRef("CurveFloat'/Game/AI/BloodCurve.BloodCurve'");
	CurveFloat = Cast<UCurveFloat>(CurveFloatRef.TryLoad());

	FOnTimelineFloat TLProgress;
	TLProgress.BindUFunction(this, FName("BloodFunction"));
	BloodLine.AddInterpFloat(CurveFloat, TLProgress);
	BloodLine.SetLooping(false);

	BloodLine.Play();

	GetWorldTimerManager().SetTimer(BloodTimer, this, &ABloodDecal::ReverseDecal, 17.5f, false);
}

void ABloodDecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BloodLine.IsPlaying())
	{
		BloodLine.TickTimeline(DeltaTime);
	}
}

void ABloodDecal::ReverseDecal()
{
	BloodLine.Reverse();
	GetWorldTimerManager().SetTimer(BloodTimer, this, &ABloodDecal::DestroySelf, 0.5f, false);
}

void ABloodDecal::DestroySelf()
{
	Destroy();
}

void ABloodDecal::BloodFunction(float Val)
{
	DynamicInstance->SetScalarParameterValue(FName("Opacity"), Val);
}