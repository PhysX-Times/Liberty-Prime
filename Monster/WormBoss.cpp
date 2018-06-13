// Fill out your copyright notice in the Description page of Project Settings.

#include "WormBoss.h"
#include "Weapon/Sword.h"
#include "TimerManager.h"
#include "Miscellaneous/AttackData.h"
#include "Miscellaneous/IndicatorDecal.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Controller/Monster/WormBossController.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"

AWormBoss::AWormBoss()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/HeroicFantasyBossesVol1/GiantWorm/Animations/GIANT_WORM_deathLow_Montage.GIANT_WORM_deathLow_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));
}

void AWormBoss::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/GiantWorm/Animations/GIANT_WORM_UndergroundJumpBiteToUnderground_Fix_Montage.GIANT_WORM_UndergroundJumpBiteToUnderground_Fix_Montage'";
	JumpAttackData = AttackData_Create(RefPath, SoundCue_Attack2, ERestriction::Restriction_Full, EImpact::Impact_Heavy, 2.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/GiantWorm/Animations/GIANT_WORM_roarPukeExhaleGazLow_Montage.GIANT_WORM_roarPukeExhaleGazLow_Montage'";
	BreatheAttackData = AttackData_Create(RefPath, SoundCue_Attack3, ERestriction::Restriction_Rot, EImpact::Impact_Light, 0.25f, 5.5f, 1.0f, false);

	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/GiantWorm/Animations/GIANT_WORM_UndergroundBite_Fix_Montage.GIANT_WORM_UndergroundBite_Fix_Montage'";
	UndergroundAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Heavy, 0.25f, 5.5f, 1.0f, false);

	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/GiantWorm/Animations/GIANT_WORM_biteLow_Montage.GIANT_WORM_biteLow_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	JawDamager = GetWorld()->SpawnActorDeferred<ASword>(JawDamagerClass, SpawnTrans, this, Instigator);
	JawDamager->MyOwner = this;
	JawDamager->DMGType = EDamageType::Damage_Poison;
	UGameplayStatics::FinishSpawningActor(JawDamager, SpawnTrans);
	JawDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("JawSocket"));

	BreatheDamager = GetWorld()->SpawnActorDeferred<ASword>(BreatheDamagerClass, SpawnTrans, this, Instigator);
	BreatheDamager->MyOwner = this;
	BreatheDamager->DMGType = EDamageType::Damage_Fire;
	UGameplayStatics::FinishSpawningActor(BreatheDamager, SpawnTrans);

	WeaponCluster.Add(JawDamager);
	WeaponCluster.Add(BreatheDamager);

	ResetMontage_Add();
}

void AWormBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsDead)
	{
		FVector SocketLoc = GetMesh()->GetSocketLocation("BreatheSocket");
		FRotator FD_TargetRot = GetMesh()->GetSocketRotation("BreatheSocket");

		BreatheDamager->GetWeaponTrailPSPoint()->SetWorldLocation(SocketLoc);
		BreatheDamager->GetWeaponTrailPSPoint()->SetWorldRotation(FQuat(FD_TargetRot));

		BreatheDamager->SetActorLocation(SocketLoc);
		FRotator FD_TargetRot_Interp = UKismetMathLibrary::RInterpTo(BreatheDamager->GetActorRotation(), FD_TargetRot, DeltaTime, 2.0f);
		BreatheDamager->SetActorRotation(FQuat(FD_TargetRot_Interp));
	}
}

void AWormBoss::Appear()
{
	if (bJumpAttack)
	{
		JumpAttack();
	}
	else
	{
		Appear_Confirm();
	}
}

void AWormBoss::Appear_Confirm()
{
	SetActorLocation(TeleportLocation);

	PlayMontage(Appear_Montage, 1.0f, ERestriction::Restriction_Full, RotLerpSpeedNormal);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	Play_SoundCue(Appear_SoundCue, true);
}

void AWormBoss::Disappear()
{
	PlayMontage(Dissappear_Montage, 1.0f, ERestriction::Restriction_Full, RotLerpSpeedNormal);
	Play_SoundCue(Disappear_SoundCue, true);
}

void AWormBoss::Disappear_Reset()
{
	ResetMontage();
}

void AWormBoss::Disappear_Confirm()
{
	if (Target)
	{
		int rand = UKismetMathLibrary::RandomIntegerInRange(0, 2);
		bFocus = false;

		if (rand == 0)
		{
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

			FTransform SpawnTrans;
			SpawnTrans.SetLocation(Target->GetMesh()->GetComponentToWorld().GetLocation());
			SpawnTrans.SetRotation(FQuat(GetActorRotation()));
			SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
			SetActorLocation(Target->GetActorLocation());

			AIndicatorDecal* MyIndicator = GetWorld()->SpawnActorDeferred<AIndicatorDecal>(IndicatorClass, SpawnTrans, this, Instigator);
			UGameplayStatics::FinishSpawningActor(MyIndicator, SpawnTrans);
			GetWorldTimerManager().SetTimer(WaitTimer, this, &AWormBoss::UndergroundAttack, 0.5f, false);
		}
		else if (rand == 2)
		{
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
			bJumpAttack = true;

			EQS_Radius = 725.0f;
			GetWorldTimerManager().SetTimer(WaitTimer, this, &AWormBoss::Run_EQS, 1.5f, false);
		}
		else
		{
			bJumpAttack = false;
			EQS_Radius = GetCapsuleComponent()->GetScaledCapsuleRadius() + Target->GetCapsuleComponent()->GetScaledCapsuleRadius() + AttackDistance_Breathe - 67.5f;
			GetWorldTimerManager().SetTimer(WaitTimer, this, &AWormBoss::Run_EQS, 1.0f, false);
		}
	}
	else
	{
		Appear_Confirm();
	}
}

void AWormBoss::JumpAttack()
{
	SetActorLocation(TeleportLocation);
	Jump_Loc = Target->GetMesh()->GetComponentToWorld().GetLocation() + FVector(0.0f, 0.0f, 2.75f);
	FRotator Jump_Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Jump_Loc);
	SetActorRotation(FQuat(FRotator(0.0f, Jump_Rot.Yaw, 0.0f)));
	Jump_Loc += GetActorForwardVector() * 32.5f;

	Attack(JumpAttackData);
}

void AWormBoss::BreatheAttack()
{
	Attack(BreatheAttackData);
}

void AWormBoss::UndergroundAttack()
{
	Crawl_Start();
	Attack(UndergroundAttackData);
}

void AWormBoss::BreatheStart()
{
	BreatheDamager->StartTrail();
	GetWorldTimerManager().SetTimer(FlameTimer, this, &AWormBoss::BreatheInit, 0.3f, false);
}

void AWormBoss::BreatheEnd()
{
	BreatheDamager->EndTrail();
	GetWorld()->GetTimerManager().ClearTimer(FlameTimer);
}

void AWormBoss::BreatheInit()
{
	ActiveSensers_Add(BreatheDamager);
	GetWorldTimerManager().SetTimer(FlameTimer, this, &AWormBoss::BreatheFunction, 0.22f, true);
}

void AWormBoss::BreatheFunction()
{
	BreatheDamager->ResetDamagedChars();
}

void AWormBoss::ResetDamager_Add()
{
	GetWorld()->GetTimerManager().ClearTimer(FlameTimer);
	GetWorld()->GetTimerManager().ClearTimer(CrawlTimer);
	GetWorld()->GetTimerManager().ClearTimer(JumpTimer);
	GetWorld()->GetTimerManager().ClearTimer(WaitTimer);
	ActiveSensers_Add(BreatheDamager);
}

void AWormBoss::ResetMontage_Add()
{
	Play_SoundCue(SoundCue_Idle_Loop, true);

	AWormBossController* AICon = Cast<AWormBossController>(GetController());

	if (AICon)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(AICon->GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(AICon->EnemyKeyID));

		if (IsTarget)
		{
			bFocus = true;
		}
		else
		{
			bFocus = false;
		}
	}
}

void AWormBoss::Crawl_Start()
{
	Play_SoundCue(SoundCue_Ground, true);
	GetWorldTimerManager().SetTimer(CrawlTimer, this, &AWormBoss::CrawlFunction, 0.1f, true);
}

void AWormBoss::Crawl_End()
{
	SoundActive_Destroy();
	GetWorld()->GetTimerManager().ClearTimer(CrawlTimer);
}

void AWormBoss::Jump_Start()
{
	Play_SoundCue(SoundCue_Ground, true);
	GetWorldTimerManager().SetTimer(JumpTimer, this, &AWormBoss::JumpFunction, 0.1f, true);

	IsJumpStart = true;
}

void AWormBoss::Jump_End()
{
	SoundActive_Destroy();
	IsJumpStart = false;
	IsJumpEnd = false;
	GetWorld()->GetTimerManager().ClearTimer(JumpTimer);
}

void AWormBoss::CrawlFunction()
{
	FTransform Trans_PS;
	Trans_PS.SetLocation(GetMesh()->GetComponentToWorld().GetLocation() + FVector(0.0f, 0.0f, 2.75f));
	Trans_PS.SetRotation(FQuat(FRotator(0.0f, UKismetMathLibrary::RandomIntegerInRange(-180, 180), 0.0f)));
	Trans_PS.SetScale3D(FVector(3.5f, 3.5f, 3.5f));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Ground_PS, Trans_PS);
}

void AWormBoss::Run_EQS_Implementation()
{

}

void AWormBoss::JumpFunction()
{
	FTransform Trans_PS;
	Trans_PS.SetRotation(FQuat(FRotator(0.0f, UKismetMathLibrary::RandomIntegerInRange(-180, 180), 0.0f)));
	Trans_PS.SetScale3D(FVector(3.5f, 3.5f, 3.5f));

	if (IsJumpStart)
	{
		Trans_PS.SetLocation(GetMesh()->GetComponentToWorld().GetLocation() + FVector(0.0f, 0.0f, 2.75f));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Ground_PS, Trans_PS);
	}

	if (IsJumpEnd)
	{
		Trans_PS.SetLocation(Jump_Loc);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Ground_PS, Trans_PS);
	}
}