// Fill out your copyright notice in the Description page of Project Settings.

#include "DarknessWarlord.h"
#include "Weapon/Sword.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

ADarknessWarlord::ADarknessWarlord()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/Enemy/DarknessWarlord/Animation/DARKNESS_WARLORD_Death_RM_Montage.DARKNESS_WARLORD_Death_RM_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj(TEXT("AnimMontage'/Game/Enemy/DarknessWarlord/Animation/DARKNESS_WARLORD_GetHit_RM_Montage.DARKNESS_WARLORD_GetHit_RM_Montage'"));
	HitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));
	HeavyHitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));
}

void ADarknessWarlord::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/Enemy/DarknessWarlord/Animation/DARKNESS_WARLORD_2hit_RM_Montage.DARKNESS_WARLORD_2hit_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/DarknessWarlord/Animation/DARKNESS_WARLORD_3hit_RM_Montage.DARKNESS_WARLORD_3hit_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/DarknessWarlord/Animation/DARKNESS_WARLORD_Attack1_RM_Montage.DARKNESS_WARLORD_Attack1_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.75f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/DarknessWarlord/Animation/DARKNESS_WARLORD_Attack2_RM_Montage.DARKNESS_WARLORD_Attack2_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.75f, 2.2f, 1.0f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	ClawDamager = GetWorld()->SpawnActorDeferred<ASword>(ClawDamagerClass, SpawnTrans, this, Instigator);
	ClawDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(ClawDamager, SpawnTrans);
	ClawDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("ClawSocket"));

	WeaponDamager = GetWorld()->SpawnActorDeferred<ASword>(WeaponDamagerClass, SpawnTrans, this, Instigator);
	WeaponDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(WeaponDamager, SpawnTrans);
	WeaponDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("WeaponSocket"));

	WeaponCluster.Add(ClawDamager);
	WeaponCluster.Add(WeaponDamager);

	UParticleSystemComponent* Weapon_FirePS = UGameplayStatics::SpawnEmitterAttached(WeaponFire_PS, GetMesh(), "ParticleSocket");
	Weapon_FirePS->SetWorldScale3D(FVector(0.25f, 0.25f, 0.25f));
	ParticleCluster.Add(Weapon_FirePS);
	Play_SoundCue(WeaponDamager->SoundCue_Fire, false, FName("ParticleSocket"));
	WeaponDamager->DMGType = EDamageType::Damage_Fire;
}

void ADarknessWarlord::Die_Add()
{
	Super::Die_Add();

	SoundComponents.Add(WeaponCue);
}