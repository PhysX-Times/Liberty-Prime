// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Landscape/Classes/Landscape.h"
#include "Engine.h"

ABaseProjectile::ABaseProjectile()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
	Senser = CreateDefaultSubobject<UBoxComponent>(TEXT("Senser"));
	Senser->SetupAttachment(RootComponent);

	PS_Scale = FVector(1.0f, 1.0f, 1.0f);
}

void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bStopCheck)
	{
		TArray<FHitResult> HitOuts;

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALibertyPrimeCharacter::StaticClass(), FoundActors);

		for (auto FoundActor : FoundActors)
		{
			ALibertyPrimeCharacter* IsLPC = Cast<ALibertyPrimeCharacter>(FoundActor);

			if (IsLPC)
			{
				if (IsLPC->faction != MyOwner->faction)
				{
					FoundActors.Remove(IsLPC);
				}
			}
		}

		TArray<AActor*> FoundWeapons;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeapon::StaticClass(), FoundWeapons);

		FoundActors.Append(FoundWeapons);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActors(FoundActors);
		CollisionQueryParams.bTraceAsyncScene = true;
		FCollisionResponseParams ResponseParams;
		ResponseParams.CollisionResponse.Destructible;

		GetWorld()->SweepMultiByChannel(HitOuts, Senser->GetComponentLocation(), Senser->GetComponentLocation() + FVector(0.0f, 0.0f, 5.5f), FQuat(Senser->GetComponentRotation()), ECollisionChannel::ECC_Destructible, FCollisionShape::MakeBox(Senser->GetScaledBoxExtent()), CollisionQueryParams, ResponseParams);

		for (auto HitOut : HitOuts)
		{
			ALibertyPrimeCharacter* IsLPC = Cast<ALibertyPrimeCharacter>(HitOut.GetActor());
			ALibertyPrimeCharacter* Arrow_Target = nullptr;

			FTransform Dest_Trans;
			Dest_Trans.SetLocation(HitOut.ImpactPoint);
			Dest_Trans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
			Dest_Trans.SetScale3D(PS_Scale);

			if (Hit_PS)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Hit_PS, Dest_Trans);
			}

			if (Hit_Cue)
			{
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Hit_Cue, Dest_Trans.GetLocation());
			}

			if (!bExplosive)
			{
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
						Arrow_Target = IsLPC;
						DamagedChars.AddUnique(IsLPC);
					}
				}
			}
			else if (bExplosive)
			{
				MyOwner->MassDamage(Explosive_radius, AttackData, Dest_Trans.GetLocation(), weapon_type);
				//DrawDebugSphere(GetWorld(), Dest_Trans.GetLocation(), Explosive_radius, 12, FColor::Red, false, 2.0f, 0, 2.2f);
			}

			if (bDestroy_OnHit)
			{
				Destroy();
			}

			if (bArrow)
			{
				if (Arrow_Target)
				{
					AttachToComponent(IsLPC->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, HitOut.BoneName);
				}
				else
				{
					AttachToActor(HitOut.GetActor(), FAttachmentTransformRules::KeepWorldTransform);

					FRotator normal_rot = HitOut.ImpactNormal.Rotation() - FRotator(180.0f, 0.0f, 0.0f);

					float rand_pitch = 0.0f;


					if (GetActorRotation().Pitch > normal_rot.Pitch)
					{
						rand_pitch = UKismetMathLibrary::RandomFloatInRange(normal_rot.Pitch, GetActorRotation().Pitch);
					}
					else
					{
						rand_pitch = UKismetMathLibrary::RandomFloatInRange(GetActorRotation().Pitch, normal_rot.Pitch);
					}

					FRotator NewRot = FRotator(rand_pitch, GetActorRotation().Yaw, GetActorRotation().Roll);
					SetActorRotation(FQuat(NewRot));
				}

				bStopCheck = true;
				break;
			}
		}
	}
}