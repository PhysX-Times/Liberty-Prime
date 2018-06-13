// Fill out your copyright notice in the Description page of Project Settings.

#include "Ghoul.h"
#include "Weapon/Sword.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

AGhoul::AGhoul()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/Enemy/Ghoul/Animation/GHOUL_Death_RM_Montage.GHOUL_Death_RM_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj(TEXT("AnimMontage'/Game/Enemy/Ghoul/Animation/GHOUL_Hit_Montage.GHOUL_Hit_Montage'"));
	HitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));
	HeavyHitDatas.Add(FMontageData(HitMontageObj.Object, 0.8f));

	faction = EFaction::Bot;
	MaxHealth = 125.0f;
	Health = 125.0f;
	AttackDistance = 197.5f;
	Weight = 72.5f;

	RunSpeed = 400.0f;
	WalkSpeed = 100.0f;

	CanBeNormalHit = true;
	CanBeHeavyHit = true;
	CanHeavyHitRot = false;
	bHeavyInvincible = false;
	bHeavyRigidBody = false;
	bSpawnBlood = true;

	BloodSize_Decal = 0.6f;
	BloodColor = FLinearColor(0.325000, 0.257029,0.025682, 1.000000);

	montage_stop_cond = EICondition::Condition_None;
	push_cond = EICondition::Condition_Both;
}

void AGhoul::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;
	
	RefPath = "AnimMontage'/Game/Enemy/Ghoul/Animation/GHOUL_JumpAttack_RM_Montage.GHOUL_JumpAttack_RM_Montage'";
	JumpAttackData = AttackData_Create(RefPath, SoundCue_GetHit, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.75f, 3.7f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/Ghoul/Animation/GHOUL_2Hit_RM_Montage.GHOUL_2Hit_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 3.7f, 0.8f, false);

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

void AGhoul::JumpAttack()
{
	Attack(JumpAttackData);
}