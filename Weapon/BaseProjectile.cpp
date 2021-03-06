// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProjectile.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
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
	PS_Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("PS_Arrow"));
	PS_Arrow->SetupAttachment(RootComponent);


	Senser->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Senser->bGenerateOverlapEvents = false;

	PS_Scale = FVector(1.0f, 1.0f, 1.0f);
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (Trail_PS)
	{
		FTransform Trans_PS;
		Trans_PS.SetLocation(PS_Arrow->GetComponentLocation());
		Trans_PS.SetRotation(PS_Arrow->GetComponentQuat());
		Trans_PS.SetScale3D(PS_Arrow->GetComponentScale());

		PS_TrailComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Trail_PS, Trans_PS);
	}

	GetWorldTimerManager().SetTimer(DestroyTimer, this, &ABaseProjectile::DestroyFunction, Destroy_Delay, false);
}

void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PS_TrailComp)
	{
		PS_TrailComp->SetWorldLocation(PS_Arrow->GetComponentToWorld().GetLocation());
		PS_TrailComp->SetWorldRotation(PS_Arrow->GetComponentToWorld().GetRotation());
	}

	if (!bStopCheck && MyOwner)
	{
		TArray<FHitResult> HitOuts;

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.bTraceAsyncScene = true;

		GetWorld()->SweepMultiByChannel(HitOuts, Senser->GetComponentLocation(), Senser->GetComponentLocation() + FVector(0.0f, 0.0f, 5.5f), FQuat(Senser->GetComponentRotation()), ECollisionChannel::ECC_Destructible, FCollisionShape::MakeBox(Senser->GetScaledBoxExtent()), CollisionQueryParams);

		for (auto HitOut : HitOuts)
		{
			ALibertyPrimeCharacter* IsLPC = Cast<ALibertyPrimeCharacter>(HitOut.GetActor());
			ALibertyPrimeCharacter* Arrow_Target = nullptr;

			FTransform Dest_Trans;
			Dest_Trans.SetLocation(HitOut.ImpactPoint + HitOut.ImpactNormal * 37.5f);
			Dest_Trans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
			Dest_Trans.SetScale3D(PS_Scale);

			if (!bExplosive)
			{
				if (IsLPC && !IsLPC->IsDead && !IsLPC->IsInvincible && MyOwner->faction != IsLPC->faction)
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
				if (!IsLPC || IsLPC && IsLPC->faction != MyOwner->faction)
				{
					if (Hit_PS)
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Hit_PS, Dest_Trans);
					}

					if (Hit_Cue)
					{
						UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Hit_Cue, Dest_Trans.GetLocation());
					}

					DestroyFunction();
				}
			}

			if (bArrow)
			{
				if (IsLPC && IsLPC->faction != MyOwner->faction && !IsLPC->IsInvincible)
				{
					AttachToComponent(IsLPC->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, HitOut.BoneName);

					bStopCheck = true;
					break;
				}
				else if (!IsLPC)
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

					bStopCheck = true;
					break;
				}
			}
		}
	}
}

void ABaseProjectile::DestroyFunction()
{
	if (PS_TrailComp)
	{
		PS_TrailComp->Deactivate();
	}

	Destroy();
}