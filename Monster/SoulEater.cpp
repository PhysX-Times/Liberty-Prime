// Fill out your copyright notice in the Description page of Project Settings.

#include "SoulEater.h"
#include "Weapon/Sword.h"
#include "Weapon/Blow.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

ASoulEater::ASoulEater()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/Enemy/SoulEater/Animation/SoulEater_Death_RM_Montage.SoulEater_Death_RM_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj(TEXT("AnimMontage'/Game/Enemy/SoulEater/Animation/SoulEater_GetHit_RM_Montage.SoulEater_GetHit_RM_Montage'"));
	HitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));
	HeavyHitDatas.Add(FMontageData(HitMontageObj.Object, 0.75f));
}

void ASoulEater::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/Enemy/SoulEater/Animation/SoulEater_Attack_RM_Montage.SoulEater_Attack_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/SoulEater/Animation/SoulEater_Roar_RM_Montage.SoulEater_Roar_RM_Montage'";
	SpitAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Light, 1.0f, 4.7f, 1.0f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	ClawDamager = GetWorld()->SpawnActorDeferred<ASword>(ClawDamagerClass, SpawnTrans, this, Instigator);
	ClawDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(ClawDamager, SpawnTrans);
	ClawDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("ClawSocket"));

	WeaponCluster.Add(ClawDamager);
}

void ASoulEater::SpitVenom()
{
	Attack(SpitAttackData);
}

void ASoulEater::Spit_notify()
{
	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetMesh()->GetSocketLocation("SpitSocket"));
	SpawnTrans.SetRotation(FQuat(GetActorRotation()));
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	ABlow* BlowDamager = GetWorld()->SpawnActorDeferred<ABlow>(BlowDamagerClass, SpawnTrans, this, Instigator);
	BlowDamager->MyOwner = this;
	BlowDamager->DMGType = EDamageType::Damage_Poison;
	UGameplayStatics::FinishSpawningActor(BlowDamager, SpawnTrans);
}