// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Miscellaneous/AttackData.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"

ABullet::ABullet()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
	Senser = CreateDefaultSubobject<UBoxComponent>(TEXT("Senser"));
	Senser->SetupAttachment(RootComponent);
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

			for (auto DamagedChar : DamagedChars)
			{
				if (DamagedChar == IsLPC)
				{
					IsDamaged = true;
					break;
				}
			}

			if (!IsDamaged)
			{
				IsLPC->Damager(MyOwner->BakeDamageData(MyOwner, AttackData, DMGType, weapon_type), IsLPC->GetMesh()->GetBoneLocation(HitOut.BoneName));
				DamagedChars.AddUnique(IsLPC);
			}
		}
	}
}