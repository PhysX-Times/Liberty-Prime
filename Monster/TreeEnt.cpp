// Fill out your copyright notice in the Description page of Project Settings.

#include "TreeEnt.h"
#include "Weapon/Sword.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"

ATreeEnt::ATreeEnt()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/Enemy/Tree/Animation/TreeEnt_Death_Montage.TreeEnt_Death_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj(TEXT("AnimMontage'/Game/Enemy/Tree/Animation/TreeEnt_Hit_Montage.TreeEnt_Hit_Montage'"));
	HeavyHitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));
}

void ATreeEnt::BeginPlay()
{
	Super::BeginPlay();

	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(1), this);

	int rand = UKismetMathLibrary::RandomIntegerInRange(0, 4);

	switch (rand)
	{
	case 1:
		DynamicMaterial->SetVectorParameterValue(FName("EmissiveColor"), FLinearColor(1.000000, 0.000000, 0.009362, 1.000000));
		break;
	case 2:
		DynamicMaterial->SetVectorParameterValue(FName("EmissiveColor"), FLinearColor(0.198116, 0.000000, 1.000000, 1.000000));
		break;
	case 3:
		DynamicMaterial->SetVectorParameterValue(FName("EmissiveColor"), FLinearColor(0.000000, 1.000000, 0.443412, 1.000000));
		break;
	}

	GetMesh()->SetMaterial(1, DynamicMaterial);

	FString RefPath;

	RefPath = "AnimMontage'/Game/Enemy/Tree/Animation/TreeEnt_Attack_BH_Montage.TreeEnt_Attack_BH_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 0.8f, false);

	RefPath = "AnimMontage'/Game/Enemy/Tree/Animation/TreeEnt_Attack_RH_Montage.TreeEnt_Attack_RH_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 0.8f, false);

	RefPath = "AnimMontage'/Game/Enemy/Tree/Animation/TreeEnt_LH_Montage.TreeEnt_LH_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 0.8f, false);

	RefPath = "AnimMontage'/Game/Enemy/Tree/Animation/TreeEnt_BL_Montage.TreeEnt_BL_Montage'";
	StompData = AttackData_Create(RefPath, SoundCue_Roar, ERestriction::Restriction_Full, EImpact::Impact_Heavy, 1.75f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/Tree/Animation/TreeEntRoar_Montage.TreeEntRoar_Montage'";
	RoarData = AttackData_Create(RefPath, SoundCue_Roar, ERestriction::Restriction_Full, EImpact::Impact_Heavy, 1.75f, 2.2f, 1.0f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	RightHandDamager = GetWorld()->SpawnActorDeferred<ASword>(HandDamagerClass, SpawnTrans, this, Instigator);
	RightHandDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(RightHandDamager, SpawnTrans);
	RightHandDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightHandSocket"));

	LeftHandDamager = GetWorld()->SpawnActorDeferred<ASword>(HandDamagerClass, SpawnTrans, this, Instigator);
	LeftHandDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(LeftHandDamager, SpawnTrans);
	LeftHandDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftHandSocket"));

	WeaponCluster.Add(RightHandDamager);
	WeaponCluster.Add(LeftHandDamager);
}

void ATreeEnt::Roar()
{
	Attack(RoarData);
}

void ATreeEnt::Stomp()
{
	Attack(StompData);
}

void ATreeEnt::Roar_Notify()
{
	MassDamage_Local(400.0f, RoarPS, FVector(1.8f, 1.8f, 1.8f), EWeaponType::Weapon_Sharp);
}

void ATreeEnt::Stomp_Notify()
{
	MassDamage_Local(400.0f, StompPS, FVector(3.0f, 3.0f, 3.0f), EWeaponType::Weapon_Blunt);
}

void ATreeEnt::MassDamage_Local(float radius, UParticleSystem* TargetPS, FVector Scale, EWeaponType WeaponType)
{
	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = TraceStart - FVector(0.0f, 0.0f, 375.0f);
	FHitResult HitOut;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	CollisionQueryParams.bTraceAsyncScene = true;
	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.Visibility;

	GetWorld()->LineTraceSingleByChannel(HitOut, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams, ResponseParams);

	if (HitOut.bBlockingHit)
	{
		TraceEnd = HitOut.ImpactPoint + HitOut.ImpactNormal * 12.5f;
		FTransform PSTrans;
		PSTrans.SetLocation(TraceEnd);
		PSTrans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
		PSTrans.SetScale3D(Scale);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TargetPS, PSTrans, true);
		MassDamage(radius, AttackData, TraceEnd, WeaponType);
	}
}