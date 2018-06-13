// Fill out your copyright notice in the Description page of Project Settings.

#include "BomberBug.h"
#include "Weapon/Sword.h"
#include "Weapon/Blow.h"
#include "Components/CapsuleComponent.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"

ABomberBug::ABomberBug()
{
	DeathLine_RefPath = "CurveFloat'/Game/AI/DeathCurve_Bug.DeathCurve_Bug'";
}

void ABomberBug::BeginPlay()
{
	Super::BeginPlay();

	int rand = UKismetMathLibrary::RandomIntegerInRange(0, BomberBug_Mats.Num() - 1);

	GetMesh()->SetMaterial(0, BomberBug_Mats[rand].BomberBugMat);
	GetMesh()->SetMaterial(1, BomberBug_Mats[rand].BomberBugAddOnsMat);

	FString RefPath;

	RefPath = "AnimMontage'/Game/Enemy/BomberBug/AnimationRM/BomberBug_Spit_RM_Montage.BomberBug_Spit_RM_Montage'";
	SpitAttackData = AttackData_Create(RefPath, SoundCue_Spit, ERestriction::Restriction_Rot, EImpact::Impact_Light, 0.5f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/BomberBug/AnimationRM/BomberBug_Attack1_RM_Montage.BomberBug_Attack1_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/Enemy/BomberBug/AnimationRM/BomberBug_Attack2_RM_Montage.BomberBug_Attack2_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Horn, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	JawDamager = GetWorld()->SpawnActorDeferred<ASword>(JawDamagerClass, SpawnTrans, this, Instigator);
	JawDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(JawDamager, SpawnTrans);
	JawDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("JawSocket"));

	WeaponCluster.Add(JawDamager);
}

void ABomberBug::SpitAttack()
{
	Attack(SpitAttackData);
}

void ABomberBug::Spit_Notify()
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

void ABomberBug::Explode_Notify()
{
	FTransform Trans_PS;
	Trans_PS.SetLocation(GetMesh()->GetComponentLocation());
	Trans_PS.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
	Trans_PS.SetScale3D(FVector(1.25f, 1.25f, 1.25f));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explode_PS, Trans_PS);

	DMGType = EDamageType::Damage_Poison;
	MassDamage(300.0f, AttackData_Create(SoundCue_Attack, EImpact::Impact_Heavy, 3.75f), Trans_PS.GetLocation(), EWeaponType::Weapon_None);
	Play_SoundCue_Location(SoundCue_Explode, GetActorLocation());
	GetCapsuleComponent()->DestroyComponent();
}

void ABomberBug::Die_Add()
{

}