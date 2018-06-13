// Fill out your copyright notice in the Description page of Project Settings.

#include "Fungoid.h"
#include "Weapon/Sword.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

AFungoid::AFungoid()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/Enemy/Fungoid/Animation/FUNGOID_Death_RM_Montage.FUNGOID_Death_RM_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj(TEXT("AnimMontage'/Game/Enemy/Fungoid/Animation/FUNGOID_Hit_RM_Montage.FUNGOID_Hit_RM_Montage'"));
	HitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));
	HeavyHitDatas.Add(FMontageData(HitMontageObj.Object, 0.75f));


	faction = EFaction::Bot;
	MaxHealth = 125.0f;
	Health = 125.0f;
	AttackDistance = 227.5f;
	Weight = 72.5f;

	RunSpeed = 400.0f;
	WalkSpeed = 80.0f;

	CanBeNormalHit = true;
	CanBeHeavyHit = true;
	CanHeavyHitRot = false;
	bHeavyInvincible = false;
	bHeavyRigidBody = false;
	bSpawnBlood = true;

	BloodSize_Decal = 0.66f;
	BloodColor = FLinearColor(0.226791, 0.041425, 0.270000, 1.000000);

	montage_stop_cond = EICondition::Condition_None;
	push_cond = EICondition::Condition_Both;
}

void AFungoid::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/Enemy/Fungoid/Animation/FUNGOID_JumpAttack_RM_Montage.FUNGOID_JumpAttack_RM_Montage'";
	JumpAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.5f, 3.7f, 0.7f, false);

	RefPath = "AnimMontage'/Game/Enemy/Fungoid/Animation/FUNGOID_2Hit_RM_Montage.FUNGOID_2Hit_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 3.7f, 0.6f, false);
	RefPath = "AnimMontage'/Game/Enemy/Fungoid/Animation/FUNGOID_Attack1_RM_Montage.FUNGOID_Attack1_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 3.7f, 0.6f, false);
	RefPath = "AnimMontage'/Game/Enemy/Fungoid/Animation/FUNGOID_Attack2_RM_Montage.FUNGOID_Attack2_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 0.75f, 3.7f, 0.6f, false);
	RefPath = "AnimMontage'/Game/Enemy/Fungoid/Animation/FUNGOID_Attack3_RM_Montage.FUNGOID_Attack3_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 3.7f, 0.6f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	StingDamager = GetWorld()->SpawnActorDeferred<ASword>(DamagerClass, SpawnTrans, this, Instigator);
	StingDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(StingDamager, SpawnTrans);
	StingDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("StingSocket"));

	WeaponCluster.Add(StingDamager);
}

void AFungoid::JumpAttack()
{
	Attack(JumpAttackData);
}