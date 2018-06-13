// Fill out your copyright notice in the Description page of Project Settings.

#include "DragonWorm.h"
#include "Weapon/Sword.h"
#include "TimerManager.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

ADragonWorm::ADragonWorm()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/Enemy/Dragonworm/Animation/Worm_Death_RM_Montage.Worm_Death_RM_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj(TEXT("AnimMontage'/Game/Enemy/Dragonworm/Animation/Worm_GetHit_RM_Montage.Worm_GetHit_RM_Montage'"));
	HitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));
	HeavyHitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));
}

void ADragonWorm::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/Enemy/Dragonworm/Animation/Worm_Attack1_RM_Montage.Worm_Attack1_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/Enemy/Dragonworm/Animation/Worm_Attack2_RM_Montage.Worm_Attack2_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Heavy, 2.5f, 2.2f, 1.0f, false);

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

void ADragonWorm::Appear()
{
	SetActorLocation(TeleportLocation);
	PlayMontage(Appear_Montage, 1.0f, ERestriction::Restriction_Full, RotLerpSpeedNormal);
	Play_SoundCue(Appear_SoundCue, true);
}

void ADragonWorm::Disappear()
{
	CanBeHeavyHit = false;
	PlayMontage(Dissappear_Montage, 1.0f, ERestriction::Restriction_Full, RotLerpSpeedNormal);
	Play_SoundCue(Disappear_SoundCue, true);
}

void ADragonWorm::Disappear_Reset()
{
	CanBeHeavyHit = true;
	ResetMontage();
}

void ADragonWorm::Disappear_Confirm()
{
	Run_EQS();
}

void ADragonWorm::Run_EQS_Implementation()
{

}


void ADragonWorm::Crawl_Start()
{
	Play_SoundCue(SoundCue_Ground, true);
	GetWorldTimerManager().SetTimer(CrawlTimer, this, &ADragonWorm::CrawlFunction, 0.1f, true);
}

void ADragonWorm::Crawl_End()
{
	SoundActive_Destroy();
	GetWorld()->GetTimerManager().ClearTimer(CrawlTimer);
}

void ADragonWorm::CrawlFunction()
{
	FTransform Trans_PS;
	Trans_PS.SetLocation(GetActorLocation() - FVector(0.0f, 0.0f, (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2.0f + 37.5f)));
	Trans_PS.SetRotation(FQuat(FRotator(0.0f, UKismetMathLibrary::RandomIntegerInRange(-180, 180), 0.0f)));
	Trans_PS.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Ground_PS, Trans_PS);
}

void ADragonWorm::ResetDamager_Add()
{
	GetWorldTimerManager().ClearTimer(CrawlTimer);
}