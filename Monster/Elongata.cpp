// Fill out your copyright notice in the Description page of Project Settings.

#include "Elongata.h"
#include "Weapon/Sword.h"
#include "Weapon/Blow.h"
#include "TimerManager.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

AElongata::AElongata()
{

}

void AElongata::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/Enemy/Elongata/Animation/Elongata_Spit_Montage.Elongata_Spit_Montage'";
	SpitAttackData = AttackData_Create(RefPath, SoundCue_Spit, ERestriction::Restriction_Rot, EImpact::Impact_Light, 0.5f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/Elongata/Animation/Elongata_Bite_Montage.Elongata_Bite_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	JawDamager = GetWorld()->SpawnActorDeferred<ASword>(JawDamagerClass, SpawnTrans, this, Instigator);
	JawDamager->MyOwner = this;
	JawDamager->DMGType = EDamageType::Damage_Poison;
	UGameplayStatics::FinishSpawningActor(JawDamager, SpawnTrans);
	JawDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("JawSocket"));

	WeaponCluster.Add(JawDamager);
}

void AElongata::SpitAttack()
{
	Attack(SpitAttackData);
}

void AElongata::Spit_Notify()
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

void AElongata::Appear()
{
	SetActorLocation(TeleportLocation);
	PlayMontage(Appear_Montage, 1.0f, ERestriction::Restriction_Full, RotLerpSpeedNormal);
	Play_SoundCue(Appear_SoundCue, true);
}

void AElongata::Disappear()
{
	CanBeHeavyHit = false;
	PlayMontage(Dissappear_Montage, 1.0f, ERestriction::Restriction_Full, RotLerpSpeedNormal);
	Play_SoundCue(Disappear_SoundCue, true);
}

void AElongata::Disappear_Reset()
{
	CanBeHeavyHit = true;
	ResetMontage();
}

void AElongata::Disappear_Confirm()
{
	Run_EQS();
}

void AElongata::Run_EQS_Implementation()
{

}

void AElongata::Crawl_Start()
{
	Play_SoundCue(SoundCue_Ground, true);
	GetWorldTimerManager().SetTimer(CrawlTimer, this, &AElongata::CrawlFunction, 0.1f, true);
}

void AElongata::Crawl_End()
{
	SoundActive_Destroy();
	GetWorld()->GetTimerManager().ClearTimer(CrawlTimer);
}

void AElongata::CrawlFunction()
{
	FTransform Trans_PS;
	Trans_PS.SetLocation(GetActorLocation() - FVector(0.0f, 0.0f, (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2.0f + 37.5f)));
	Trans_PS.SetRotation(FQuat(FRotator(0.0f, UKismetMathLibrary::RandomIntegerInRange(-180, 180), 0.0f)));
	Trans_PS.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Ground_PS, Trans_PS);
}

void AElongata::ResetDamager_Add()
{
	GetWorldTimerManager().ClearTimer(CrawlTimer);
}