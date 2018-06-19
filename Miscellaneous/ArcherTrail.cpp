// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcherTrail.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"
#include "Weapon/HomingProjectile.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/MeshComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Engine.h"

// Sets default values
AArcherTrail::AArcherTrail()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Bow = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bow"));
	Bow->SetupAttachment(Mesh, FName("BowSocket"));

	Quiver = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Quiver"));
	Quiver->SetupAttachment(Mesh, FName("QuiverSocket"));

	Charge_Trans_Comp = CreateDefaultSubobject<UArrowComponent>(TEXT("Charge_Trans_Comp"));
	Charge_Trans_Comp->SetupAttachment(RootComponent);

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->bGenerateOverlapEvents = false;

	Quiver->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Quiver->bGenerateOverlapEvents = false;

	Opacity_Meshes.Add(Mesh);
	Opacity_Meshes.Add(Bow);
	Opacity_Meshes.Add(Quiver);

	can_rot = false;
}

// Called when the game starts or when spawned
void AArcherTrail::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat ProgressFunction;
	ProgressFunction.BindUFunction(this, FName("OpacityFunction"));

	Opacityline.AddInterpFloat(OpacityCurve, ProgressFunction);
	Opacityline.SetLooping(false);

	Opacityline.PlayFromStart();

	Mesh->GetAnimInstance()->Montage_Play(Mesh_Charge_Montage);
	Bow->GetAnimInstance()->Montage_Play(Bow_Charge_Montage);
}

// Called every frame
void AArcherTrail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Opacityline.IsPlaying())
	{
		Opacityline.TickTimeline(DeltaTime);
	}

	if (MyOwner && can_rot)
	{
		float tar_yaw = GetActorRotation().Yaw + Rot_Speed;

		SetActorLocation(Rot_Offset.RotateAngleAxis(tar_yaw, FVector(0, 0, 1)) + MyOwner->GetActorLocation() - FVector(0.0f, 0.0f, MyOwner->Cap_Height));
		SetActorRotation(FQuat(FRotator(0.0f, GetActorRotation().Yaw + Rot_Speed, 0.0f)));
	}
}

void AArcherTrail::Charge_Notify()
{
	Charge_PSComp = UGameplayStatics::SpawnEmitterAttached(Charge_PS, Charge_Trans_Comp);

	GetWorldTimerManager().ClearTimer(ChargeTimer);
	GetWorldTimerManager().SetTimer(ChargeTimer, this, &AArcherTrail::Charge_Function, 0.05f, false);

	Opacityline.Reverse();
}

void AArcherTrail::Charge_Function()
{
	if (charge_count < 125 && MyOwner)
	{
		int rand_y = UKismetMathLibrary::RandomIntegerInRange(-75, 75);
		int rand_z = UKismetMathLibrary::RandomIntegerInRange(-75, 75);

		FTransform SpawnTrans;
		SpawnTrans.SetLocation(GetActorRightVector() * rand_y + GetActorUpVector() * rand_z + GetActorForwardVector() * 137.5f + Charge_Trans_Comp->GetComponentLocation());
		SpawnTrans.SetRotation(FQuat(GetActorRotation()));
		SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		AHomingProjectile* ArrowProjectile = GetWorld()->SpawnActorDeferred<AHomingProjectile>(ArrowProjectileClass, SpawnTrans, this, Instigator);
		ArrowProjectile->MyOwner = MyOwner;
		ArrowProjectile->DMGType = EDamageType::Damage_Fire;
		ArrowProjectile->AttackData = ArrowAttackData;
		ArrowProjectile->bArrow = true;
		ArrowProjectile->Destroy_Delay = 2.5f;
		UGameplayStatics::FinishSpawningActor(ArrowProjectile, SpawnTrans);

		if (charge_count % 8 == 0)
		{
			MyOwner->Play_SoundCue(SoundCue_Arrow, false, FName("None"), 1.0f, ArrowProjectile->GetRootScene());
		}

		charge_count += 1;

		GetWorldTimerManager().ClearTimer(ChargeTimer);
		GetWorldTimerManager().SetTimer(ChargeTimer, this, &AArcherTrail::Charge_Function, 0.02f, false);
	}
	else
	{
		FTransform Trans_PS;
		Trans_PS.SetLocation(Charge_Trans_Comp->GetComponentLocation());
		Trans_PS.SetRotation(FQuat(GetActorRotation()));
		Trans_PS.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		Charge_PSComp->DestroyComponent();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Charge_End_PS, Trans_PS);

		Destroy();
	}
}

void AArcherTrail::OpacityFunction(float Value)
{
	for (auto Opacity_Mesh : Opacity_Meshes)
	{
		for (int i = 0; i < Opacity_Mesh->GetNumMaterials(); i++)
		{
			UMaterialInstanceDynamic* DynamicInstance = Opacity_Mesh->CreateAndSetMaterialInstanceDynamic(i);
			DynamicInstance->SetScalarParameterValue(FName("Opacity"), Value);
		}
	}
}