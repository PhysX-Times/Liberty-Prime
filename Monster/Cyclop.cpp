// Fill out your copyright notice in the Description page of Project Settings.

#include "Cyclop.h"
#include "Weapon/Sword.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

ACyclop::ACyclop()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/Enemy/Cyclop/Animation/Cyclop_Death_Montage.Cyclop_Death_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj(TEXT("AnimMontage'/Game/Enemy/Cyclop/Animation/Cyclop_GetHit_Montage.Cyclop_GetHit_Montage'"));
	HeavyHitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));
}

void ACyclop::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/Enemy/Cyclop/Animation/Cyclop_Attack1_Montage.Cyclop_Attack1_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 0.8f, false);

	RefPath = "AnimMontage'/Game/Enemy/Cyclop/Animation/Cyclop_Attack2_Montage.Cyclop_Attack2_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 0.8f, false);

	RefPath = "AnimMontage'/Game/Enemy/Cyclop/Animation/Cyclop_Attack3_Montage.Cyclop_Attack3_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 0.8f, false);

	RefPath = "AnimMontage'/Game/Enemy/Cyclop/Animation/Cyclop_Attack4_Montage.Cyclop_Attack4_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 0.8f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	HandDamager = GetWorld()->SpawnActorDeferred<ASword>(HandDamagerClass, SpawnTrans, this, Instigator);
	HandDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(HandDamager, SpawnTrans);
	HandDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("HandSocket"));

	LegDamager = GetWorld()->SpawnActorDeferred<ASword>(LegDamagerClass, SpawnTrans, this, Instigator);
	LegDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(LegDamager, SpawnTrans);
	LegDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LegSocket"));

	WeaponCluster.Add(HandDamager);
	WeaponCluster.Add(LegDamager);

	int rand = UKismetMathLibrary::RandomIntegerInRange(0, 4);

	switch (rand)
	{
	case 1:
		HandDamager->ApplyEffect(EDamageType::Damage_Fire);
		break;
	case 2:
		HandDamager->ApplyEffect(EDamageType::Damage_Freeze);
		break;
	case 3:
		HandDamager->ApplyEffect(EDamageType::Damage_Poison);
		break;
	default:
		break;
	}
}