// Fill out your copyright notice in the Description page of Project Settings.

#include "WereSnake.h"
#include "Weapon/Sword.h"
#include "Weapon/Blow.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

AWereSnake::AWereSnake()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/Enemy/Weresnake/Animation/WERESNAKE_Death_RM_Montage.WERESNAKE_Death_RM_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj(TEXT("AnimMontage'/Game/Enemy/Weresnake/Animation/WERESNAKE_Hit_RM_Montage.WERESNAKE_Hit_RM_Montage'"));
	HitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));
	HeavyHitDatas.Add(FMontageData(HitMontageObj.Object, 0.75f));
}

void AWereSnake::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;
	RefPath = "AnimMontage'/Game/Enemy/Weresnake/Animation/WERESNAKE_2HitA_RM_Montage.WERESNAKE_2HitA_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 0.7f, 4.7f, 0.75f, false);

	RefPath = "AnimMontage'/Game/Enemy/Weresnake/Animation/WERESNAKE_2HitB_RM_Montage.WERESNAKE_2HitB_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 4.7f, 0.75f, false);

	RefPath = "AnimMontage'/Game/Enemy/Weresnake/Animation/WERESNAKE_SpinAttack_RM_Montage.WERESNAKE_SpinAttack_RM_Montage'";
	SpinAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Light, 0.5f, 4.7f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/Weresnake/Animation/WERESNAKE_JumpAttack_RM_Montage.WERESNAKE_JumpAttack_RM_Montage'";
	JumpAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 4.7f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/Weresnake/Animation/WERESNAKE_SpitVenom_RM_Montage.WERESNAKE_SpitVenom_RM_Montage'";
	SpitAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Light, 1.0f, 4.7f, 1.0f, false);

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

	WeaponCluster.Add(LeftHandDamager);
	WeaponCluster.Add(RightHandDamager);

	int rand = UKismetMathLibrary::RandomIntegerInRange(1, 2);

	switch (rand)
	{
	case 1:
		LeftHandDamager->ApplyEffect(EDamageType::Damage_Fire, true);
		RightHandDamager->ApplyEffect(EDamageType::Damage_Fire);
		break;
	case 2:
		LeftHandDamager->ApplyEffect(EDamageType::Damage_Freeze, true);
		RightHandDamager->ApplyEffect(EDamageType::Damage_Freeze);
		break;
	default:
		break;
	}
}

void AWereSnake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWereSnake::SpinAttack()
{
	Attack(SpinAttackData);
}

void AWereSnake::JumpAttack()
{
	Attack(JumpAttackData);
}

void AWereSnake::SpitVenom()
{
	Attack(SpitAttackData);
}

void AWereSnake::Spit_notify()
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