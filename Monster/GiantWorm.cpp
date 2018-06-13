// Fill out your copyright notice in the Description page of Project Settings.

#include "GiantWorm.h"
#include "Weapon/Sword.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Controller/Monster/GiantWormController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"

AGiantWorm::AGiantWorm()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/Enemy/GiantWorm/Animation/Giant_Worm_Death_Montage.Giant_Worm_Death_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj(TEXT("AnimMontage'/Game/Enemy/GiantWorm/Animation/Giant_Worm_GetHitBody_Montage.Giant_Worm_GetHitBody_Montage'"));
	HitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));
	HeavyHitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> Mat1(TEXT("MaterialInstanceConstant'/Game/Enemy/GiantWorm/Material/GiantWormMat_Black.GiantWormMat_Black'"));
	Mats.Add(Mat1.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> Mat2(TEXT("MaterialInstanceConstant'/Game/Enemy/GiantWorm/Material/GiantWormMat_Fire.GiantWormMat_Fire'"));
	Mats.Add(Mat2.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> Mat3(TEXT("MaterialInstanceConstant'/Game/Enemy/GiantWorm/Material/GiantWormMat_Green.GiantWormMat_Green'"));
	Mats.Add(Mat3.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> Mat4(TEXT("MaterialInstanceConstant'/Game/Enemy/GiantWorm/Material/GiantWormMat_Ice.GiantWormMat_Ice'"));
	Mats.Add(Mat4.Object);
}

void AGiantWorm::BeginPlay()
{
	Super::BeginPlay();

	int rand = UKismetMathLibrary::RandomIntegerInRange(0, Mats.Num() - 1);
	UMaterialInstance* Mat_Inst = Mats[rand];
	GetMesh()->SetMaterial(0, Mat_Inst);

	FString RefPath;

	RefPath = "AnimMontage'/Game/Enemy/GiantWorm/Animation/Giant_Worm_Attack2_Montage.Giant_Worm_Attack2_Montage'";
	CircleAttackData = AttackData_Create(RefPath, SoundCue_Attack2, ERestriction::Restriction_Rot, EImpact::Impact_Heavy, 2.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/GiantWorm/Animation/Giant_Worm_Attack3_Montage.Giant_Worm_Attack3_Montage'";
	BreatheAttackData = AttackData_Create(RefPath, SoundCue_Attack3, ERestriction::Restriction_Rot, EImpact::Impact_Light, 0.25f, 5.5f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/GiantWorm/Animation/Giant_Worm_Attack1_Montage.Giant_Worm_Attack1_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	JawDamager = GetWorld()->SpawnActorDeferred<ASword>(JawDamagerClass, SpawnTrans, this, Instigator);
	JawDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(JawDamager, SpawnTrans);
	JawDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("JawSocket"));

	BreatheDamager = GetWorld()->SpawnActorDeferred<ASword>(BreatheDamagerClass, SpawnTrans, this, Instigator);
	BreatheDamager->MyOwner = this;
	BreatheDamager->DMGType = EDamageType::Damage_Poison;
	UGameplayStatics::FinishSpawningActor(BreatheDamager, SpawnTrans);

	WeaponCluster.Add(JawDamager);
	WeaponCluster.Add(BreatheDamager);

	ResetMontage_Add();
}

void AGiantWorm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsDead)
	{
		FVector SocketLoc = GetMesh()->GetSocketLocation("BreatheSocket");
		FRotator FD_TargetRot = GetMesh()->GetSocketRotation("BreatheSocket");

		BreatheDamager->GetWeaponTrailPSPoint()->SetWorldLocation(SocketLoc);
		BreatheDamager->GetWeaponTrailPSPoint()->SetWorldRotation(FQuat(FD_TargetRot));

		BreatheDamager->SetActorLocation(SocketLoc);
		FRotator FD_TargetRot_Interp = UKismetMathLibrary::RInterpTo(BreatheDamager->GetActorRotation(), FD_TargetRot, DeltaTime, 5.5f);
		BreatheDamager->SetActorRotation(FQuat(FD_TargetRot_Interp));
	}
}

void AGiantWorm::Appear()
{
	SetActorLocation(TeleportLocation);
	PlayMontage(Appear_Montage, 1.0f, ERestriction::Restriction_Full, RotLerpSpeedNormal);
	Play_SoundCue(Appear_SoundCue, true);
}

void AGiantWorm::Disappear()
{
	CanBeHeavyHit = false;
	PlayMontage(Dissappear_Montage, 1.0f, ERestriction::Restriction_Full, RotLerpSpeedNormal);
	Play_SoundCue(Disappear_SoundCue, true);
}

void AGiantWorm::Disappear_Reset()
{
	CanBeHeavyHit = true;
	ResetMontage();
}

void AGiantWorm::Disappear_Confirm()
{
	Run_EQS();
}

void AGiantWorm::CircleAttack()
{
	Attack(CircleAttackData);
}

void AGiantWorm::BreatheAttack()
{
	Attack(BreatheAttackData);
}

void AGiantWorm::BreatheStart()
{
	BreatheDamager->StartTrail();
	GetWorldTimerManager().SetTimer(FlameTimer, this, &AGiantWorm::BreatheInit, 0.3f, false);
}

void AGiantWorm::BreatheEnd()
{
	BreatheDamager->EndTrail();
	GetWorld()->GetTimerManager().ClearTimer(FlameTimer);
}

void AGiantWorm::BreatheInit()
{
	ActiveSensers_Add(BreatheDamager);
	GetWorldTimerManager().SetTimer(FlameTimer, this, &AGiantWorm::BreatheFunction, 0.22f, true);
}

void AGiantWorm::BreatheFunction()
{
	BreatheDamager->ResetDamagedChars();
}

void AGiantWorm::Breathe_Check()
{
	AGiantWormController* IsCon = Cast<AGiantWormController>(GetController());

	if (IsCon)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(IsCon->GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(IsCon->EnemyKeyID));

		if (IsTarget)
		{
			float Distance = (GetActorLocation() - IsTarget->GetActorLocation()).Size() - GetCapsuleComponent()->GetScaledCapsuleRadius() - IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();

			if (Distance > AttackDistance || Distance < 430.0f)
			{
				GetMesh()->GetAnimInstance()->Montage_Stop(0.25f);
				ResetMontage();
				ResetDamager();
			}
		}
	}
}

void AGiantWorm::ResetDamager_Add()
{
	GetWorld()->GetTimerManager().ClearTimer(FlameTimer);
	GetWorld()->GetTimerManager().ClearTimer(GroundTimer);
	GetWorld()->GetTimerManager().ClearTimer(CrawlTimer);
	ActiveSensers_Add(BreatheDamager);
}

void AGiantWorm::Ground_Start()
{
	Play_SoundCue(SoundCue_Ground, true);
	GetWorldTimerManager().SetTimer(GroundTimer, this, &AGiantWorm::GroundFunction, 0.05f, true);
}

void AGiantWorm::Ground_End()
{
	SoundActive_Destroy();
	GetWorld()->GetTimerManager().ClearTimer(GroundTimer);
}

void AGiantWorm::GroundFunction()
{
	for (auto SocketName_Ground : SocketNames_Ground)
	{
		FVector TraceStart = GetMesh()->GetSocketLocation(SocketName_Ground) + FVector(0.0f, 0.0f, 37.5f);
		FVector TraceEnd = GetMesh()->GetSocketLocation(SocketName_Ground) - FVector(0.0f, 0.0f, 127.5f);
		FHitResult HitOut;

		TArray<AActor*> FoundActors;
		TArray<AActor*> AllActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeapon::StaticClass(), AllActors);
		FoundActors.Append(AllActors);
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALibertyPrimeCharacter::StaticClass(), AllActors);
		FoundActors.Append(AllActors);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActors(FoundActors);
		CollisionQueryParams.bTraceAsyncScene = true;

		GetWorld()->LineTraceSingleByChannel(HitOut, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);

		if (HitOut.bBlockingHit)
		{
			FTransform Trans_PS;
			Trans_PS.SetLocation(HitOut.ImpactNormal * 17.5f + HitOut.ImpactPoint);
			Trans_PS.SetRotation(FQuat(FRotator(0.0f, UKismetMathLibrary::RandomIntegerInRange(-180, 180), 0.0f)));
			Trans_PS.SetScale3D(FVector(1.25f, 1.25f, 1.25f));
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Ground_PS, Trans_PS);
		}
	}
}

void AGiantWorm::ResetMontage_Add()
{
	Play_SoundCue(SoundCue_Idle_Loop, true);
}

void AGiantWorm::Crawl_Start()
{
	Play_SoundCue(SoundCue_Ground, true);
	GetWorldTimerManager().SetTimer(CrawlTimer, this, &AGiantWorm::CrawlFunction, 0.1f, true);
}

void AGiantWorm::Crawl_End()
{
	SoundActive_Destroy();
	GetWorld()->GetTimerManager().ClearTimer(CrawlTimer);
}

void AGiantWorm::CrawlFunction()
{
	FTransform Trans_PS;
	Trans_PS.SetLocation(GetActorLocation() - FVector(0.0f, 0.0f, (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2.0f + 32.5f)));
	Trans_PS.SetRotation(FQuat(FRotator(0.0f, UKismetMathLibrary::RandomIntegerInRange(-180, 180), 0.0f)));
	Trans_PS.SetScale3D(FVector(2.2f, 2.2f, 2.2f));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Ground_PS, Trans_PS);
}

void AGiantWorm::Run_EQS_Implementation()
{

}