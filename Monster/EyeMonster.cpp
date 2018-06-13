// Fill out your copyright notice in the Description page of Project Settings.

#include "EyeMonster.h"
#include "Weapon/Sword.h"
#include "TimerManager.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

AEyeMonster::AEyeMonster()
{
	SprayArrow = CreateDefaultSubobject<UArrowComponent>(FName("SprayArrow"));
	SprayArrow->SetupAttachment(RootComponent);
}

void AEyeMonster::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/Enemy/EyeMonster/AnimationRM/FlyingEye_Attack2_RM_Montage.FlyingEye_Attack2_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_BumpAttack, ERestriction::Restriction_Full, EImpact::Impact_Heavy, 2.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/Enemy/EyeMonster/AnimationRM/FlyingEye_Attack1_RM_Montage.FlyingEye_Attack1_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.25f, false);
	RefPath = "AnimMontage'/Game/Enemy/EyeMonster/AnimationRM/FlyingEye_Attack3_RM_Montage.FlyingEye_Attack3_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 0.0f, 1.25f, false);

	RefPath = "AnimMontage'/Game/Enemy/EyeMonster/AnimationRM/FlyingEye_Cast2_RM_Montage.FlyingEye_Cast2_RM_Montage'";
	SprayAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Light, 0.22f, 2.2f, 1.0f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	HeadDamager = GetWorld()->SpawnActorDeferred<ASword>(HeadDamagerClass, SpawnTrans, this, Instigator);
	HeadDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(HeadDamager, SpawnTrans);
	HeadDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("HeadSocket"));

	TentacleDamager = GetWorld()->SpawnActorDeferred<ASword>(TentacleDamagerClass, SpawnTrans, this, Instigator);
	TentacleDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(TentacleDamager, SpawnTrans);
	TentacleDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("TentacleSocket"));

	FlameDamager = GetWorld()->SpawnActorDeferred<ASword>(FlameDamagerClass, SpawnTrans, this, Instigator);
	FlameDamager->MyOwner = this;
	FlameDamager->DMGType = EDamageType::Damage_Poison;
	UGameplayStatics::FinishSpawningActor(FlameDamager, SpawnTrans);
	FlameDamager->AttachToComponent(SprayArrow, FAttachmentTransformRules::SnapToTargetIncludingScale);

	WeaponCluster.Add(HeadDamager);
	WeaponCluster.Add(TentacleDamager);
	WeaponCluster.Add(FlameDamager);
}

void AEyeMonster::SprayAttack()
{
	Attack(SprayAttackData);
}

void AEyeMonster::FlameStart()
{
	PoisonSigil_Comp = UGameplayStatics::SpawnEmitterAttached(PoisonSigil_PS, SprayArrow);
	PoisonSigil_Comp->SetRelativeScale3D(FVector(0.65f, 0.65f, 0.65f));

	FlameDamager->StartTrail();
	Play_SoundCue(SoundCue_SprayAttack, true);
	GetWorldTimerManager().SetTimer(FlameTimer, this, &AEyeMonster::FlameInit, 0.1f, false);
}

void AEyeMonster::FlameEnd()
{
	Destroy_Sigil();

	FlameDamager->EndTrail();
	GetWorld()->GetTimerManager().ClearTimer(FlameTimer);
	SoundComponents.Add(SoundComponent_Active);
}

void AEyeMonster::FlameInit()
{
	ActiveSensers_Add(FlameDamager);
	GetWorldTimerManager().SetTimer(FlameTimer, this, &AEyeMonster::FlameFunction, 0.15f, true);
}

void AEyeMonster::FlameFunction()
{
	FlameDamager->ResetDamagedChars();
}

void AEyeMonster::Start_Movement()
{
	restriction = ERestriction::Restriction_Move_NonRot;
}

void AEyeMonster::ResetDamager_Add()
{
	Destroy_Sigil();

	GetWorld()->GetTimerManager().ClearTimer(FlameTimer);
	ActiveSensers_Add(FlameDamager);
}

void AEyeMonster::Destroy_Sigil()
{
	if (PoisonSigil_Comp)
	{
		PoisonSigil_Comp->Deactivate();
	}

	FTransform Trans_PS;
	Trans_PS.SetLocation(SprayArrow->GetComponentLocation());
	Trans_PS.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
	Trans_PS.SetScale3D(FVector(0.65f, 0.65f, 0.65f));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Sigil_Destroy_PS, Trans_PS);
}