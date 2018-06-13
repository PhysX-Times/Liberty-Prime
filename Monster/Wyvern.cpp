// Fill out your copyright notice in the Description page of Project Settings.

#include "Wyvern.h"
#include "Weapon/Sword.h"
#include "TimerManager.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

AWyvern::AWyvern()
{
	
}

void AWyvern::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;
	RefPath = "AnimMontage'/Game/Enemy/Wyvern/Animation/WYVERN_StingerAttack_Anim_Montage.WYVERN_StingerAttack_Anim_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Heavy, 2.0f, 2.5f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/Wyvern/Animation/WYVERN_BiteAttack_Anim_Montage.WYVERN_BiteAttack_Anim_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.5f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/Wyvern/Animation/WYVERN_SpreadFire_Anim_Montage.WYVERN_SpreadFire_Anim_Montage'";
	FlameThrowAttack = AttackData_Create(RefPath, SoundCue_FireBreathe, ERestriction::Restriction_Rot, EImpact::Impact_Light, 0.2f, 3.0f, 1.0f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	BiteDamager = GetWorld()->SpawnActorDeferred<ASword>(BiteDamagerClass, SpawnTrans, this, Instigator);
	BiteDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(BiteDamager, SpawnTrans);
	BiteDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("BiteSocket"));

	StingerDamager = GetWorld()->SpawnActorDeferred<ASword>(StingerDamagerClass, SpawnTrans, this, Instigator);
	StingerDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(StingerDamager, SpawnTrans);
	StingerDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("StingerSocket"));

	FlameDamager = GetWorld()->SpawnActorDeferred<ASword>(FlameDamagerClass, SpawnTrans, this, Instigator);
	FlameDamager->MyOwner = this;
	FlameDamager->DMGType = EDamageType::Damage_Fire;
	UGameplayStatics::FinishSpawningActor(FlameDamager, SpawnTrans);

	WeaponCluster.Add(FlameDamager);
	WeaponCluster.Add(BiteDamager);
	WeaponCluster.Add(StingerDamager);
}

void AWyvern::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsDead)
	{
		FVector SocketLoc = GetMesh()->GetSocketLocation("FlameSocket");
		FRotator FD_TargetRot = FRotator(0.0f, GetMesh()->GetSocketRotation("FlameSocket").Yaw, 0.0f);

		FlameDamager->GetWeaponTrailPSPoint()->SetWorldLocation(SocketLoc);
		FlameDamager->GetWeaponTrailPSPoint()->SetWorldRotation(FQuat(FD_TargetRot));

		FlameDamager->SetActorLocation(SocketLoc);
		FRotator FD_TargetRot_Interp = UKismetMathLibrary::RInterpTo(FlameDamager->GetActorRotation(), FD_TargetRot, DeltaTime, 5.5f);
		FlameDamager->SetActorRotation(FQuat(FRotator(0.0f, FD_TargetRot_Interp.Yaw, 0.0f)));
	}
}

void AWyvern::FlameThrow()
{
	Attack(FlameThrowAttack);
}

void AWyvern::FlameStart()
{
	FlameDamager->StartTrail();
	GetWorldTimerManager().SetTimer(FlameTimer, this, &AWyvern::FlameInit, 0.4f, false);
}

void AWyvern::FlameEnd()
{
	FlameDamager->EndTrail();
	GetWorld()->GetTimerManager().ClearTimer(FlameTimer);
}

void AWyvern::FlameInit()
{
	ActiveSensers_Add(FlameDamager);
	GetWorldTimerManager().SetTimer(FlameTimer, this, &AWyvern::FlameFunction, 0.22f, true);
}

void AWyvern::FlameFunction()
{
	FlameDamager->ResetDamagedChars();
}

void AWyvern::ResetDamager_Add()
{
	GetWorld()->GetTimerManager().ClearTimer(FlameTimer);
	ActiveSensers_Add(FlameDamager);
}

void AWyvern::ResetMontage_Add()
{

}