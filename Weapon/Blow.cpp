// Fill out your copyright notice in the Description page of Project Settings.

#include "Blow.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

ABlow::ABlow()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
	Senser = CreateDefaultSubobject<UBoxComponent>(TEXT("Senser"));
	Senser->SetupAttachment(RootComponent);
	BlowPSPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("BlowPSPoint"));
	BlowPSPoint->SetupAttachment(RootComponent);

	Senser->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Senser->bGenerateOverlapEvents = false;

	TimeElapsed = 0.0f;
}

void ABlow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeElapsed += DeltaTime;

	TArray<FHitResult> HitOuts;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(MyOwner);
	CollisionQueryParams.bTraceAsyncScene = true;

	GetWorld()->SweepMultiByChannel(HitOuts, Senser->GetComponentLocation(), Senser->GetComponentLocation(), FQuat(Senser->GetComponentRotation()), ECollisionChannel::ECC_Destructible, FCollisionShape::MakeBox(Senser->GetScaledBoxExtent()), CollisionQueryParams);

	for (auto HitOut : HitOuts)
	{
		ALibertyPrimeCharacter* IsLPC = Cast<ALibertyPrimeCharacter>(HitOut.GetActor());

		if (IsLPC && IsLPC != MyOwner && !IsLPC->IsDead && !IsLPC->IsInvincible && MyOwner->faction != IsLPC->faction)
		{
			bool IsDamaged = false;
			float Distance = (GetActorLocation() - IsLPC->GetActorLocation()).Size();

			float TargetDelay = FMath::Clamp(MaxDelay * Distance / MaxDistance, 0.0f, MaxDelay);

			for (auto DamagedChar : DamagedChars)
			{
				if (DamagedChar == IsLPC)
				{
					IsDamaged = true;
					break;
				}
			}

			if (!IsDamaged && TimeElapsed >= TargetDelay)
			{
				IsLPC->Damager(MyOwner->BakeDamageData(MyOwner, MyOwner->AttackData, DMGType, weapon_type), IsLPC->GetMesh()->GetBoneLocation(HitOut.BoneName));
				DamagedChars.AddUnique(IsLPC);
			}
		}
	}
}

void ABlow::BeginPlay()
{
	Super::BeginPlay();

	FTransform BlowTrans;
	BlowTrans.SetLocation(BlowPSPoint->GetComponentLocation());
	BlowTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
	BlowTrans.SetRotation(FQuat(BlowPSPoint->GetComponentRotation()));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BlowPS, BlowTrans, true);
}