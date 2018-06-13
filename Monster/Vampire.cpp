// Fill out your copyright notice in the Description page of Project Settings.

#include "Vampire.h"
#include "Weapon/Sword.h"
#include "TimerManager.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

AVampire::AVampire()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/Enemy/Vampire/Animation/VAMPIRE_death_RM_Montage.VAMPIRE_death_RM_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj(TEXT("AnimMontage'/Game/Enemy/Vampire/Animation/VAMPIRE_getHitLight_RM_Montage.VAMPIRE_getHitLight_RM_Montage'"));
	HitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitHeavyMontageObj(TEXT("AnimMontage'/Game/Enemy/Vampire/Animation/VAMPIRE_getHitHeavy_RM_Montage.VAMPIRE_getHitHeavy_RM_Montage'"));
	HeavyHitDatas.Add(FMontageData(HitHeavyMontageObj.Object, 1.0f));
}

void AVampire::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/Enemy/Vampire/Animation/VAMPIRE_attack1_RM_Montage.VAMPIRE_attack1_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 0.75f, false);

	RefPath = "AnimMontage'/Game/Enemy/Vampire/Animation/VAMPIRE_attack2_RM_Montage.VAMPIRE_attack2_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 0.75f, false);

	RefPath = "AnimMontage'/Game/Enemy/Vampire/Animation/VAMPIRE_attack3_RM_Montage.VAMPIRE_attack3_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 0.75f, false);

	RefPath = "AnimMontage'/Game/Enemy/Vampire/Animation/VAMPIRE_roar_RM_Montage.VAMPIRE_roar_RM_Montage'";
	GasAttack = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Light, 0.5f, 2.2f, 1.0f, false);

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

	GasDamager = GetWorld()->SpawnActorDeferred<ASword>(GasDamagerClass, SpawnTrans, this, Instigator);
	GasDamager->MyOwner = this;
	GasDamager->DMGType = EDamageType::Damage_Poison;
	UGameplayStatics::FinishSpawningActor(GasDamager, SpawnTrans);

	WeaponCluster.Add(GasDamager);
	WeaponCluster.Add(RightClawDamager);
	WeaponCluster.Add(LeftClawDamager);
}

void AVampire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsDead)
	{
		FVector SocketLoc = GetMesh()->GetSocketLocation("GasSocket");
		FRotator FD_TargetRot = FRotator(0.0f, GetMesh()->GetSocketRotation("GasSocket").Yaw, 0.0f);

		GasDamager->GetWeaponTrailPSPoint()->SetWorldLocation(SocketLoc);
		GasDamager->GetWeaponTrailPSPoint()->SetWorldRotation(FQuat(FD_TargetRot));

		GasDamager->SetActorLocation(SocketLoc);
		FRotator FD_TargetRot_Interp = UKismetMathLibrary::RInterpTo(GasDamager->GetActorRotation(), FD_TargetRot, DeltaTime, 5.5f);
		GasDamager->SetActorRotation(FQuat(FRotator(0.0f, FD_TargetRot_Interp.Yaw, 0.0f)));
	}
}

void AVampire::ExhaleGas()
{
	Attack(GasAttack);
}

void AVampire::GasStart()
{
	GasDamager->StartTrail();
	GetWorldTimerManager().SetTimer(GasTimer, this, &AVampire::GasInit, 0.4f, false);
}

void AVampire::GasEnd()
{
	GasDamager->EndTrail();
	GetWorld()->GetTimerManager().ClearTimer(GasTimer);
}

void AVampire::GasInit()
{
	ActiveSensers_Add(GasDamager);
	GetWorldTimerManager().SetTimer(GasTimer, this, &AVampire::GasFunction, 0.22f, true);
}

void AVampire::GasFunction()
{
	GasDamager->ResetDamagedChars();
}

void AVampire::ResetDamager_Add()
{
	GetWorld()->GetTimerManager().ClearTimer(GasTimer);
	ActiveSensers_Add(GasDamager);
}