// Fill out your copyright notice in the Description page of Project Settings.

#include "BoneDragon.h"
#include "Weapon/Sword.h"
#include "TimerManager.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

ABoneDragon::ABoneDragon()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/Enemy/BoneDragon/Animation/BoneDragon_Death_RM_Montage.BoneDragon_Death_RM_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj(TEXT("AnimMontage'/Game/Enemy/BoneDragon/Animation/BoneDragon_Hit_RM_Montage.BoneDragon_Hit_RM_Montage'"));
	HitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));
	HeavyHitDatas.Add(FMontageData(HitMontageObj.Object, 0.75f));

	faction = EFaction::Bot;
	MaxHealth = 225.0f;
	Health = 225.0f;
	AttackDistance = 217.5f;
	Weight = 37.5f;

	RunSpeed = 700.0f;
	WalkSpeed = 175.0f;

	DMG = 25.0f;

	CanBeNormalHit = true;
	CanBeHeavyHit = true;
	CanHeavyHitRot = false;
	bHeavyInvincible = false;
	bHeavyRigidBody = false;

	montage_stop_cond = EICondition::Condition_None;
	push_cond = EICondition::Condition_Both;
}

void ABoneDragon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsDead)
	{
		float TargetYaw = UKismetMathLibrary::NormalizedDeltaRotator(TargetRot, GetActorRotation()).Yaw;
		OffsetYaw = FMath::Clamp(UKismetMathLibrary::FInterpTo(OffsetYaw, TargetYaw, DeltaTime, 9.25f), -120.0f, 120.0f);
		
		FVector SocketLoc = GetMesh()->GetSocketLocation("FlameSocket");
		FRotator FD_TargetRot = FRotator(0.0f, GetActorRotation().Yaw + OffsetYaw, 0.0f);

		FlameDamager->GetWeaponTrailPSPoint()->SetWorldLocation(SocketLoc);
		FlameDamager->GetWeaponTrailPSPoint()->SetWorldRotation(FQuat(FD_TargetRot));

		FlameDamager->SetActorLocation(SocketLoc);
		FRotator FD_TargetRot_Interp = UKismetMathLibrary::RInterpTo(FlameDamager->GetActorRotation(), FD_TargetRot, DeltaTime, 3.7f);
		FlameDamager->SetActorRotation(FQuat(FRotator(0.0f, FD_TargetRot_Interp.Yaw, 0.0f)));
	}
}

void ABoneDragon::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/Enemy/BoneDragon/Animation/BoneDragon_Slash_L_RM_Montage.BoneDragon_Slash_L_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/BoneDragon/Animation/BoneDragon_Slash_R_RM_Montage.BoneDragon_Slash_R_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/BoneDragon/Animation/BoneDragon_Bite_RM_Montage.BoneDragon_Bite_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/BoneDragon/Animation/BoneDragon_FlameThrow_RM_Montage.BoneDragon_FlameThrow_RM_Montage'";
	FlameThrowAttack = AttackData_Create(RefPath, SoundCue_FireBreathe, ERestriction::Restriction_Rot, EImpact::Impact_Light, 0.22f, 2.2f, 1.0f, false);

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

	BiteDamager = GetWorld()->SpawnActorDeferred<ASword>(BiteDamagerClass, SpawnTrans, this, Instigator);
	BiteDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(BiteDamager, SpawnTrans);
	BiteDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("BiteSocket"));

	FlameDamager = GetWorld()->SpawnActorDeferred<ASword>(FlameDamagerClass, SpawnTrans, this, Instigator);
	FlameDamager->MyOwner = this;
	FlameDamager->DMGType = EDamageType::Damage_Fire;
	UGameplayStatics::FinishSpawningActor(FlameDamager, SpawnTrans);

	WeaponCluster.Add(FlameDamager);
	WeaponCluster.Add(BiteDamager);
	WeaponCluster.Add(LeftHandDamager);
	WeaponCluster.Add(RightHandDamager);
}

void ABoneDragon::FlameThrow()
{
	Attack(FlameThrowAttack);
}

void ABoneDragon::FlameStart()
{
	FlameDamager->StartTrail();
	Play_SoundCue(SoundCue_FireBreathe, true);
	GetWorldTimerManager().SetTimer(FlameTimer, this, &ABoneDragon::FlameInit, 0.3f, false);
}

void ABoneDragon::FlameEnd()
{
	FlameDamager->EndTrail();
	GetWorld()->GetTimerManager().ClearTimer(FlameTimer);
	SoundComponents.Add(SoundComponent_Active);
}

void ABoneDragon::FlameInit()
{
	ActiveSensers_Add(FlameDamager);
	GetWorldTimerManager().SetTimer(FlameTimer, this, &ABoneDragon::FlameFunction, 0.22f, true);
}

void ABoneDragon::FlameFunction()
{
	FlameDamager->ResetDamagedChars();
}

void ABoneDragon::ResetDamager_Add()
{
	GetWorld()->GetTimerManager().ClearTimer(FlameTimer);
	ActiveSensers_Add(FlameDamager);
}