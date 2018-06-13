// Fill out your copyright notice in the Description page of Project Settings.

#include "FatOgre.h"
#include "Weapon/Sword.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

AFatOgre::AFatOgre()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/Enemy/FatOgre/Animation/FAT_OGRE_Death_RM_Montage.FAT_OGRE_Death_RM_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj(TEXT("AnimMontage'/Game/Enemy/FatOgre/Animation/FAT_OGRE_Hit_RM_Montage.FAT_OGRE_Hit_RM_Montage'"));
	HitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));
	HeavyHitDatas.Add(FMontageData(HitMontageObj.Object, 0.75f));

	faction = EFaction::Bot;
	MaxHealth = 200.0f;
	Health = 200.0f;
	AttackDistance = 237.5f;
	Weight = 37.5f;

	DMG = 25.0f;

	RunSpeed = 320.0f;
	WalkSpeed = 80.0f;

	CanBeNormalHit = true;
	CanBeHeavyHit = true;
	CanHeavyHitRot = false;
	bHeavyInvincible = false;
	bHeavyRigidBody = false;
	bSpawnBlood = true;

	BloodSize_Decal = 1.0f;

	montage_stop_cond = EICondition::Condition_None;
	push_cond = EICondition::Condition_Both;

	DMGType = EDamageType::Damage_Fire;
}

void AFatOgre::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/Enemy/FatOgre/Animation/FAT_OGRE_Attack1_RM_Montage.FAT_OGRE_Attack1_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/FatOgre/Animation/FAT_OGRE_Attack2_RM_Montage.FAT_OGRE_Attack2_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/FatOgre/Animation/FAT_OGRE_Attack3_RM_Montage.FAT_OGRE_Attack3_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.5f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/FatOgre/Animation/FAT_OGRE_Roar_RM_Montage.FAT_OGRE_Roar_RM_Montage'";
	RoarData = AttackData_Create(RefPath, SoundCue_Roar, ERestriction::Restriction_Full, EImpact::Impact_Heavy, 1.75f, 2.2f, 1.0f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	HandDamager = GetWorld()->SpawnActorDeferred<ASword>(HandDamagerClass, SpawnTrans, this, Instigator);
	HandDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(HandDamager, SpawnTrans);
	HandDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("ClubSocket"));

	WeaponCluster.Add(HandDamager);
}

void AFatOgre::Roar()
{
	Attack(RoarData);
}

void AFatOgre::RoarNotify()
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
		TraceEnd = HitOut.ImpactPoint + HitOut.ImpactNormal * 17.5f;
		FTransform PSTrans;
		PSTrans.SetLocation(TraceEnd);
		PSTrans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
		PSTrans.SetScale3D(FVector(1.4f, 1.4f, 1.4f));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RoarPS, PSTrans, true);
		MassDamage(417.5f, AttackData, TraceEnd, EWeaponType::Weapon_Sharp);
	}
}