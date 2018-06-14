// Fill out your copyright notice in the Description page of Project Settings.

#include "SummonCaster.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
//#include "DrawDebugHelpers.h"

// Sets default values
ASummonCaster::ASummonCaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	DMGType = EDamageType::Damage_None;
	WeaponType = EWeaponType::Weapon_None;

	PS_Rot = FRotator(0.0f, 0.0f, 0.0f);
	PS_Scale = FVector(1.0f, 1.0f, 1.0f);
	PS_Offset = FVector(0.0f, 0.0f, 0.0f);
}

// Called when the game starts or when spawned
void ASummonCaster::BeginPlay()
{
	Super::BeginPlay();
	
	Mesh->GetAnimInstance()->Montage_Play(Cast_Montage);
}

void ASummonCaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Opacity_Current = FMath::Clamp(Opacity_Current + Opacity_Speed, 0.0f, 1.0f);

	for (int i = 0; i < Mesh->GetNumMaterials(); i++)
	{
		UMaterialInstanceDynamic* DynamicInstance = Mesh->CreateAndSetMaterialInstanceDynamic(i);
		DynamicInstance->SetScalarParameterValue(FName("Opacity"), Opacity_Current);
	}
}

void ASummonCaster::Cast_Notify()
{
	FTransform Trans_PS;
	Trans_PS.SetLocation(GetActorLocation() + PS_Offset);
	Trans_PS.SetRotation(FQuat(PS_Rot));
	Trans_PS.SetScale3D(PS_Scale);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion_PS, Trans_PS);
	MyOwner->Play_SoundCue_Location(SoundCue_Explosion, Trans_PS.GetLocation());

	MyOwner->MassDamage(Explosion_Radius, AttackData, Trans_PS.GetLocation(), WeaponType, this, true, DMGType);
	//DrawDebugSphere(GetWorld(), Trans_PS.GetLocation(), Explosion_Radius, 9, FColor::Red, false, 3.5f, 0, 1.1f);

	Destroy();
}