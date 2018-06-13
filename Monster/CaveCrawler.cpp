// Fill out your copyright notice in the Description page of Project Settings.

#include "CaveCrawler.h"
#include "Weapon/Sword.h"
#include "TimerManager.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"

ACaveCrawler::ACaveCrawler()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/Enemy/Cavecrawler/Animation/CAVECRAWLER_Death_RM_Montage.CAVECRAWLER_Death_RM_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj(TEXT("AnimMontage'/Game/Enemy/Cavecrawler/Animation/CAVECRAWLER_Hit_RM_Montage.CAVECRAWLER_Hit_RM_Montage'"));
	HitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));
	HeavyHitDatas.Add(FMontageData(HitMontageObj.Object, 0.75f));

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> TrollMat1(TEXT("MaterialInstanceConstant'/Game/Enemy/Cavecrawler/Material/CavecrawlerMat_Inst.CavecrawlerMat_Inst'"));
	CaveCrawlerMats.Add(TrollMat1.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> TrollMat2(TEXT("MaterialInstanceConstant'/Game/Enemy/Cavecrawler/Material/CavecrawlerMat_Inst1.CavecrawlerMat_Inst1'"));
	CaveCrawlerMats.Add(TrollMat2.Object);

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

void ACaveCrawler::BeginPlay()
{
	Super::BeginPlay();

	int rand = UKismetMathLibrary::RandomIntegerInRange(0, CaveCrawlerMats.Num() - 1);
	GetMesh()->SetMaterial(0, CaveCrawlerMats[rand]);

	FString RefPath;
	RefPath = "AnimMontage'/Game/Enemy/Cavecrawler/Animation/CAVECRAWLER_AttackLeft_RM_Montage.CAVECRAWLER_AttackLeft_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/Cavecrawler/Animation/CAVECRAWLER_AttackRight_RM_Montage.CAVECRAWLER_AttackRight_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/Cavecrawler/Animation/CAVECRAWLER_StingerAttack_RM_Montage.CAVECRAWLER_StingerAttack_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/Cavecrawler/Animation/CAVECRAWLER_ExhaleGas_RM_Montage.CAVECRAWLER_ExhaleGas_RM_Montage'";
	GasAttack = AttackData_Create(RefPath, SoundCue_Exhale, ERestriction::Restriction_Rot, EImpact::Impact_Light, 0.5f, 2.2f, 1.0f, false);

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

	StingDamager = GetWorld()->SpawnActorDeferred<ASword>(StingDamagerClass, SpawnTrans, this, Instigator);
	StingDamager->MyOwner = this;
	StingDamager->DMGType = EDamageType::Damage_Poison;
	UGameplayStatics::FinishSpawningActor(StingDamager, SpawnTrans);
	StingDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("StingSocket"));

	GasDamager = GetWorld()->SpawnActorDeferred<ASword>(GasDamagerClass, SpawnTrans, this, Instigator);
	GasDamager->MyOwner = this;
	GasDamager->DMGType = EDamageType::Damage_Poison;
	UGameplayStatics::FinishSpawningActor(GasDamager, SpawnTrans);

	WeaponCluster.Add(GasDamager);
	WeaponCluster.Add(StingDamager);
	WeaponCluster.Add(LeftHandDamager);
	WeaponCluster.Add(RightHandDamager);
}

void ACaveCrawler::Tick(float DeltaTime)
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

void ACaveCrawler::ExhaleGas()
{
	Attack(GasAttack);
}

void ACaveCrawler::GasStart()
{
	GasDamager->StartTrail();
	GetWorldTimerManager().SetTimer(GasTimer, this, &ACaveCrawler::GasInit, 0.4f, false);
}

void ACaveCrawler::GasEnd()
{
	GasDamager->EndTrail();
	GetWorld()->GetTimerManager().ClearTimer(GasTimer);
}

void ACaveCrawler::GasInit()
{
	ActiveSensers_Add(GasDamager);
	GetWorldTimerManager().SetTimer(GasTimer, this, &ACaveCrawler::GasFunction, 0.22f, true);
}

void ACaveCrawler::GasFunction()
{
	GasDamager->ResetDamagedChars();
}

void ACaveCrawler::ResetDamager_Add()
{
	GetWorld()->GetTimerManager().ClearTimer(GasTimer);
	ActiveSensers_Add(GasDamager);
}