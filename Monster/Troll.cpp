// Fill out your copyright notice in the Description page of Project Settings.

#include "Troll.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Miscellaneous/AttackData.h"
#include "Weapon/Decorator.h"
#include "Weapon/Sword.h"

ATroll::ATroll()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("/Game/InfinityBladeAdversaries/Enemy/Enemy_Troll/Enemy_Troll_Poison_Animation/TrollDeathMontage"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HeavyHitMontageObj(TEXT("AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Troll/Enemy_Troll_Poison_Animation/TrollHeavyHitMontage.TrollHeavyHitMontage'"));
	HeavyHitDatas.Add(FMontageData(HeavyHitMontageObj.Object, 1.0f));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj(TEXT("AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Troll/Enemy_Troll_Poison_Animation/TrollHitMontage.TrollHitMontage'"));
	HitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> TrollMat1(TEXT("MaterialInstanceConstant'/Game/InfinityBladeAdversaries/Enemy/Enemy_Troll/Materials/CharM_Troll_Inst.CharM_Troll_Inst'"));
	TrollMats.Add(TrollMat1.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> TrollMat2(TEXT("MaterialInstanceConstant'/Game/InfinityBladeAdversaries/Enemy/Enemy_Troll/Materials/CharM_Troll_Poison_Inst.CharM_Troll_Poison_Inst'"));
	TrollMats.Add(TrollMat2.Object);


	faction = EFaction::Bot;

	Health = 175.0f;
	MaxHealth = 175.0f;
	AttackDistance = 225.0;
	BloodSize_Decal = 0.85f;
	RunSpeed = 125.0f;
	WalkSpeed = 125.0f;
	Weight = 33.5f;
	DMG = 20.0f;

	CanBeNormalHit = true;
	CanBeHeavyHit = true;
	CanHeavyHitRot = false;
	bHeavyInvincible = false;
	bHeavyRigidBody = false;
	bSpawnBlood = true;

	BloodColor = FLinearColor(0.014005, 0.220000, 0.021817, 1.000000);

	montage_stop_cond = EICondition::Condition_None;
	push_cond = EICondition::Condition_Both;

	DMGType = EDamageType::Damage_Poison;
}

void ATroll::BeginPlay()
{
	Super::BeginPlay();

	int rand = UKismetMathLibrary::RandomIntegerInRange(0, TrollMats.Num() - 1);
	GetMesh()->SetMaterial(0, TrollMats[rand]);

	FString RefPath;

	RefPath = "AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Troll/Enemy_Troll_Poison_Animation/TrollDashMontage.TrollDashMontage'";
	DashAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Heavy, 2.0f, 4.4f, 1.0f, false);

	RefPath = "AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Troll/Enemy_Troll_Poison_Animation/TrollSwipeMontage.TrollSwipeMontage'";
	SwipeAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Heavy, 1.75f, 3.7f, 1.0f, false);

	RefPath = "AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Troll/Enemy_Troll_Poison_Animation/TrollHeadOverMontage.TrollHeadOverMontage'";
	HeadOverAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Heavy, 2.0f, 3.7f, 1.0f, false);

	RefPath = "AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Troll/Enemy_Troll_Poison_Animation/TrollRoar.TrollRoar'";
	RoarAttackData = AttackData_Create(RefPath, SoundCue_Roar, ERestriction::Restriction_Full, EImpact::Impact_Heavy, 2.0f, 3.3f, 1.0f, false);

	RefPath = "AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Troll/Enemy_Troll_Poison_Animation/TrollAttackMontage_1.TrollAttackMontage_1'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Troll/Enemy_Troll_Poison_Animation/TrollAttackMontage_2.TrollAttackMontage_2'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	RightHandDamager = GetWorld()->SpawnActorDeferred<ASword>(RightHandDamagerClass, SpawnTrans, this, Instigator);
	RightHandDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(RightHandDamager, SpawnTrans);
	RightHandDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightHandSocket"));

	LeftHandDamager = GetWorld()->SpawnActorDeferred<ASword>(LeftHandDamagerClass, SpawnTrans, this, Instigator);
	LeftHandDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(LeftHandDamager, SpawnTrans);
	LeftHandDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftHandSocket"));

	WeaponCluster.Add(LeftHandDamager);
	WeaponCluster.Add(RightHandDamager);

	rand = UKismetMathLibrary::RandomIntegerInRange(0, 1);

	if (rand == 1)
	{
		ADecorator* Helmet = GetWorld()->SpawnActorDeferred<ADecorator>(HelmetClass, SpawnTrans, this, Instigator);
		Helmet->MyOwner = this;
		UGameplayStatics::FinishSpawningActor(Helmet, SpawnTrans);
		Helmet->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("HelmetSocket"));
		WeaponCluster.Add(Helmet);
	}
}

void ATroll::SwipeAttack()
{
	Attack(SwipeAttackData);
}

void ATroll::HeadOverAttack()
{
	Attack(HeadOverAttackData);
}

void ATroll::DashAttack()
{
	Attack(DashAttackData);
}

void ATroll::Roar()
{
	Attack(RoarAttackData);
}

void ATroll::RoarNotify()
{
	ApplyMass(GetActorLocation(), FVector(1.3f, 1.3f, 1.3f), RoarPS, 397.0f);
}

void ATroll::OverHeadNotify()
{
	ApplyMass(RightHandDamager->GetElementalPSComp()->GetComponentLocation(), FVector(1.2f, 1.2f, 1.2f), OverHeadPS, 275.0f);
}

void ATroll::ApplyMass(FVector StartLoc, FVector PS_Scale, UParticleSystem* TargetPS, float radius)
{
	FVector TraceStart = StartLoc;
	FVector TraceEnd = TraceStart - FVector(0.0f, 0.0f, 325.0f);
	FHitResult HitOut;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	CollisionQueryParams.bTraceAsyncScene = true;
	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.Visibility;

	GetWorld()->LineTraceSingleByChannel(HitOut, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams, ResponseParams);

	if (HitOut.bBlockingHit)
	{
		TraceEnd = HitOut.ImpactPoint + HitOut.ImpactNormal * 12.5f;
		FTransform PSTrans;
		PSTrans.SetLocation(TraceEnd);
		PSTrans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
		PSTrans.SetScale3D(PS_Scale);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TargetPS, PSTrans, true);
		MassDamage(radius, AttackData, TraceEnd, EWeaponType::Weapon_None);
	}
}