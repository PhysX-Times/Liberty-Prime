// Fill out your copyright notice in the Description page of Project Settings.

#include "Sword.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Miscellaneous/AttackData.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

ASword::ASword()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
	Senser = CreateDefaultSubobject<UBoxComponent>(TEXT("Senser"));
	Senser->SetupAttachment(RootComponent);
	WeaponTrailPSPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("WeaponTrailPSPoint"));
	WeaponTrailPSPoint->SetupAttachment(RootComponent);
	ElementalPSPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ElementalPSPoint"));
	ElementalPSPoint->SetupAttachment(RootComponent);
	Weapon_SK = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon_SK"));
	Weapon_SK->SetupAttachment(RootComponent);
	Weapon_ST = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon_ST"));
	Weapon_ST->SetupAttachment(RootComponent);

	DMGType = EDamageType::Damage_None;

	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue_Fire_Obj(TEXT("SoundCue'/Game/SFX/Fire_Loop_Cue.Fire_Loop_Cue'"));
	SoundCue_Fire = SoundCue_Fire_Obj.Object;
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue_Ice_Obj(TEXT("SoundCue'/Game/SFX/Ice_Loop_Cue.Ice_Loop_Cue'"));
	SoundCue_Ice = SoundCue_Ice_Obj.Object;
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue_Poison_Obj(TEXT("SoundCue'/Game/SFX/Poison_Loop_Cue.Poison_Loop_Cue'"));
	SoundCue_Poison = SoundCue_Poison_Obj.Object;
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue_Lightning_Obj(TEXT("SoundCue'/Game/SFX/Light_Loop_Cue.Light_Loop_Cue'"));
	SoundCue_Lightning = SoundCue_Lightning_Obj.Object;
}

void ASword::BeginPlay()
{
	Super::BeginPlay();

	if (Weapon_SK->SkeletalMesh)
	{
		MyOwner->BodyParts_SK.Add(Weapon_SK);
	}

	if (Weapon_ST->GetStaticMesh())
	{
		MyOwner->BodyParts_ST.Add(Weapon_ST);
	}
}

void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool IsFound = false;

	for (auto ActiveSenser : MyOwner->ActiveSensers)
	{
		if (ActiveSenser == this)
		{
			IsFound = true;
			break;
		}
	}

	if (IsFound)
	{
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
					IsLPC->Damager(MyOwner->BakeDamageData(MyOwner, MyOwner->AttackData, DMGType, weapon_type), IsLPC->GetMesh()->GetBoneLocation(HitOut.BoneName));
					DamagedChars.AddUnique(IsLPC);
				}
			}
		}
	}
}

void ASword::ResetDamagedChars()
{
	DamagedChars.Empty();
}

void ASword::StartTrail()
{
	if (WeaponTrailPSComp)
	{
		WeaponTrailPSComp->Deactivate();
	}

	switch (DMGType)
	{
	case EDamageType::Damage_Fire:
		WeaponTrailPSComp = UGameplayStatics::SpawnEmitterAttached(FireTrailPS, WeaponTrailPSPoint);
		break;
	case EDamageType::Damage_Freeze:
		WeaponTrailPSComp = UGameplayStatics::SpawnEmitterAttached(IceTrailPS, WeaponTrailPSPoint);
		break;
	case EDamageType::Damage_Poison:
		WeaponTrailPSComp = UGameplayStatics::SpawnEmitterAttached(PoisonTrailPS, WeaponTrailPSPoint);
		break;
	case EDamageType::Damage_Lightning:
		WeaponTrailPSComp = UGameplayStatics::SpawnEmitterAttached(LightningTrailPS, WeaponTrailPSPoint);
		break;
	case EDamageType::Damage_None:
		WeaponTrailPSComp = UGameplayStatics::SpawnEmitterAttached(DefaultTrailPS, WeaponTrailPSPoint);
		break;
	default:
		break;
	}
}

void ASword::EndTrail()
{
	if (WeaponTrailPSComp)
	{
		WeaponTrailPSComp->Deactivate();
	}
}

void ASword::ApplyEffect(EDamageType ElementalType, bool bSpawnSound)
{
	if (ElementalPSComp)
	{
		ElementalPSComp->Deactivate();
	}

	if (SoundComponent_Weapon)
	{
		MyOwner->SoundComponents.Add(SoundComponent_Weapon);
	}

	DMGType = ElementalType;

	switch (DMGType)
	{
	case EDamageType::Damage_Fire:

		if (bSpawnSound)
		{
			SoundComponent_Weapon = MyOwner->Play_SoundCue(SoundCue_Fire, false);
		}

		ElementalPSComp = UGameplayStatics::SpawnEmitterAttached(FirePS, ElementalPSPoint);
		break;
	case EDamageType::Damage_Freeze:

		if (bSpawnSound)
		{
			SoundComponent_Weapon = MyOwner->Play_SoundCue(SoundCue_Ice, false);
		}

		ElementalPSComp = UGameplayStatics::SpawnEmitterAttached(IcePS, ElementalPSPoint);
		break;
	case EDamageType::Damage_Poison:

		if (bSpawnSound)
		{
			SoundComponent_Weapon = MyOwner->Play_SoundCue(SoundCue_Poison, false);
		}

		ElementalPSComp = UGameplayStatics::SpawnEmitterAttached(PoisonPS, ElementalPSPoint);
		break;
	case EDamageType::Damage_Lightning:

		if (bSpawnSound)
		{
			SoundComponent_Weapon = MyOwner->Play_SoundCue(SoundCue_Lightning, false);
		}

		ElementalPSComp = UGameplayStatics::SpawnEmitterAttached(LightningPS, ElementalPSPoint);
		break;
	default:
		break;
	}
}

void ASword::Clear_PS()
{
	if (ElementalPSComp)
	{
		ElementalPSComp->Deactivate();
	}

	if (WeaponTrailPSComp)
	{
		WeaponTrailPSComp->Deactivate();
	}
}