// Fill out your copyright notice in the Description page of Project Settings.

#include "RatRogue.h"
#include "Weapon/Sword.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

ARatRogue::ARatRogue()
{

}

void ARatRogue::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	int rand = UKismetMathLibrary::RandomIntegerInRange(0, 2);

	if (rand == 0)
	{
		GetMesh()->SetSkeletalMesh(Mesh_1);
	}
	else if (rand == 1)
	{
		GetMesh()->SetSkeletalMesh(Mesh_2);
	}
	else if (rand == 2)
	{
		GetMesh()->SetSkeletalMesh(Mesh_3);
	}

	RefPath = "AnimMontage'/Game/Enemy/RatkinWarlord/AnimationRM/RatWarlord_HeavySmash_RM_Montage.RatWarlord_HeavySmash_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/Enemy/RatkinWarlord/AnimationRM/RatWarlord_ScissorsBlow_RM_Montage.RatWarlord_ScissorsBlow_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/Enemy/RatkinWarlord/AnimationRM/RatWarlord_SwingLeft_RM_Montage.RatWarlord_SwingLeft_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/Enemy/RatkinWarlord/AnimationRM/RatWarlord_SwingLeft_RM_Montage.RatWarlord_SwingLeft_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/Enemy/RatkinWarlord/AnimationRM/RatWarlord_SwingLeftR_RM_Montage.RatWarlord_SwingLeftR_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/Enemy/RatkinWarlord/AnimationRM/RatWarlord_SwingRight_RM_Montage.RatWarlord_SwingRight_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/Enemy/RatkinWarlord/AnimationRM/RatWarlord_SwingRightR_RM_Montage.RatWarlord_SwingRightR_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/Enemy/RatkinWarlord/AnimationRM/RatWarlord_Thrust_RM_Montage.RatWarlord_Thrust_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.75f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/Enemy/RatkinWarlord/AnimationRM/RatWarlord_ThrustR_RM_Montage.RatWarlord_ThrustR_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.75f, 2.2f, 1.0f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	LeftSwordDamager = GetWorld()->SpawnActorDeferred<ASword>(SwordDamagerClass, SpawnTrans, this, Instigator);
	LeftSwordDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(LeftSwordDamager, SpawnTrans);
	LeftSwordDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftSwordSocket"));

	RightSwordDamager = GetWorld()->SpawnActorDeferred<ASword>(SwordDamagerClass, SpawnTrans, this, Instigator);
	RightSwordDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(RightSwordDamager, SpawnTrans);
	RightSwordDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightSwordSocket"));

	WeaponCluster.Add(LeftSwordDamager);
	WeaponCluster.Add(RightSwordDamager);

	rand = UKismetMathLibrary::RandomIntegerInRange(0, 5);

	switch (rand)
	{
	case 1:
		LeftSwordDamager->ApplyEffect(EDamageType::Damage_Fire, true);
		RightSwordDamager->ApplyEffect(EDamageType::Damage_Fire);
		break;
	case 2:
		LeftSwordDamager->ApplyEffect(EDamageType::Damage_Freeze, true);
		RightSwordDamager->ApplyEffect(EDamageType::Damage_Freeze);
		break;
	case 3:
		LeftSwordDamager->ApplyEffect(EDamageType::Damage_Poison, true);
		RightSwordDamager->ApplyEffect(EDamageType::Damage_Poison);
		break;
	case 4:
		LeftSwordDamager->ApplyEffect(EDamageType::Damage_Lightning, true);
		RightSwordDamager->ApplyEffect(EDamageType::Damage_Lightning);
		break;
	default:
		break;
	}
}