// Fill out your copyright notice in the Description page of Project Settings.

#include "BoneDragon_Twin.h"
#include "Weapon/Sword.h"
#include "TimerManager.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

ABoneDragon_Twin::ABoneDragon_Twin()
{
	bBuildUp = true;
}

void ABoneDragon_Twin::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;
	IsInvincible = true;

	RefPath = "AnimMontage'/Game/Enemy/BoneDragon_Twin/Animation/Bone_Dragon_Attack1_Montage.Bone_Dragon_Attack1_Montage'";
	BiteAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.5f, 2.0f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/BoneDragon_Twin/Animation/Bone_Dragon_TailWhipLeft_Montage.Bone_Dragon_TailWhipLeft_Montage'";
	TailLeftAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/BoneDragon_Twin/Animation/Bone_Dragon_TailWhipRight_Montage.Bone_Dragon_TailWhipRight_Montage'";
	TailRightAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	TailDamager = GetWorld()->SpawnActorDeferred<ASword>(TailDamagerClass, SpawnTrans, this, Instigator);
	TailDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(TailDamager, SpawnTrans);
	TailDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("TailSocket"));

	BiteDamager = GetWorld()->SpawnActorDeferred<ASword>(BiteDamagerClass, SpawnTrans, this, Instigator);
	BiteDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(BiteDamager, SpawnTrans);
	BiteDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("BiteSocket"));

	WeaponCluster.Add(BiteDamager);
	WeaponCluster.Add(TailDamager);

	GetMesh()->GetAnimInstance()->Montage_Play(Rubble_Idle_Montage);
}

void ABoneDragon_Twin::Attack_Bite()
{
	Attack(BiteAttackData);
}

void ABoneDragon_Twin::Attack_Tail_Left()
{
	Attack(TailLeftAttackData);
}

void ABoneDragon_Twin::Attack_Tail_Right()
{
	Attack(TailRightAttackData);
}

void ABoneDragon_Twin::Build_Up()
{
	PlayMontage(Pose_Montage, 1.0f, ERestriction::Restriction_Full, RotLerpSpeedNormal);
	Play_SoundCue_Location(SoundCue_GetPose, GetActorLocation());
}

void ABoneDragon_Twin::Pose_End()
{
	IsInvincible = false;
	Pose_Notify();
	Breathe();
	ResetMontage();
}

void ABoneDragon_Twin::Pose_Notify_Implementation()
{

}