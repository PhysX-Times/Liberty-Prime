// Fill out your copyright notice in the Description page of Project Settings.

#include "Werewolf.h"
#include "Weapon/Sword.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

AWerewolf::AWerewolf()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/Enemy/Werewolf/Animation/WEREWOLF_Death_Anim_RM_Montage.WEREWOLF_Death_Anim_RM_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj(TEXT("AnimMontage'/Game/Enemy/Werewolf/Animation/WEREWOLF_GetHit_Anim_RM_Montage.WEREWOLF_GetHit_Anim_RM_Montage'"));
	HitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));
	HeavyHitDatas.Add(FMontageData(HitMontageObj.Object, 0.8f));
}

void AWerewolf::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/Enemy/Werewolf/Animation/WEREWOLF_JumpClawAttack_Anim_RM_Montage.WEREWOLF_JumpClawAttack_Anim_RM_Montage'";
	JumpAttackData = AttackData_Create(RefPath, SoundCue_GetHit, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 2.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/Werewolf/Animation/WEREWOLF_ClawAttack2HitComboForward_Anim_RM_Montage.WEREWOLF_ClawAttack2HitComboForward_Anim_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 0.75f, false);

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

	WeaponCluster.Add(RightClawDamager);
	WeaponCluster.Add(LeftClawDamager);
}

void AWerewolf::JumpAttack()
{
	Attack(JumpAttackData);
}