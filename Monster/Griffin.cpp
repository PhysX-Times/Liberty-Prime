// Fill out your copyright notice in the Description page of Project Settings.

#include "Griffin.h"
#include "Weapon/Sword.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

AGriffin::AGriffin()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/Enemy/Griffin/Animation/GRIFFON_Death_RM_Montage.GRIFFON_Death_RM_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj(TEXT("AnimMontage'/Game/Enemy/Griffin/Animation/GRIFFON_GetHit_RM_Montage.GRIFFON_GetHit_RM_Montage'"));
	HitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));
	HeavyHitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));
}

void AGriffin::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/Enemy/Griffin/Animation/GRIFFON_AttackL_RM_Montage.GRIFFON_AttackL_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 0.75f, false);
	RefPath = "AnimMontage'/Game/Enemy/Griffin/Animation/GRIFFON_AttackR_RM_Montage.GRIFFON_AttackR_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 0.75f, false);
	RefPath = "AnimMontage'/Game/Enemy/Griffin/Animation/GRIFFON_BleakAttack_RM_Montage.GRIFFON_BleakAttack_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.85f, 2.2f, 0.75f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	RightClawDamager = GetWorld()->SpawnActorDeferred<ASword>(ClawDamagerClass, SpawnTrans, this, Instigator);
	RightClawDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(RightClawDamager, SpawnTrans);
	RightClawDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightClawSocket"));

	LeftClawDamager = GetWorld()->SpawnActorDeferred<ASword>(ClawDamagerClass, SpawnTrans, this, Instigator);
	LeftClawDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(LeftClawDamager, SpawnTrans);
	LeftClawDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftClawSocket"));

	BleakDamager = GetWorld()->SpawnActorDeferred<ASword>(BleakDamagerClass, SpawnTrans, this, Instigator);
	BleakDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(BleakDamager, SpawnTrans);
	BleakDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("BleakSocket"));

	WeaponCluster.Add(RightClawDamager);
	WeaponCluster.Add(LeftClawDamager);
	WeaponCluster.Add(BleakDamager);

	LeftClawDamager->ApplyEffect(EDamageType::Damage_Fire, true);
	RightClawDamager->ApplyEffect(EDamageType::Damage_Fire);

	UParticleSystemComponent* TailPSComp = UGameplayStatics::SpawnEmitterAttached(TailPS, GetMesh(), FName("TailSocket"));
	TailPSComp->SetWorldScale3D(FVector(0.175f, 0.175f, 0.175f));
	ParticleCluster.Add(TailPSComp);
}