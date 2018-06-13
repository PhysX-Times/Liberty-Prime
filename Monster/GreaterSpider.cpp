// Fill out your copyright notice in the Description page of Project Settings.

#include "GreaterSpider.h"
#include "Weapon/Sword.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/PhysicalMaterials/PhysicalMaterial.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"

AGreaterSpider::AGreaterSpider()
{

}

void AGreaterSpider::BeginPlay()
{
	Super::BeginPlay();

	int rand = UKismetMathLibrary::RandomIntegerInRange(0, 1);

	if (rand == 0)
	{
		GetMesh()->SetSkeletalMesh(Mesh_1);

		rand = UKismetMathLibrary::RandomIntegerInRange(0, Mesh1_Mats.Num() - 1);
		GetMesh()->SetMaterial(0, Mesh1_Mats[rand]);
	}
	else
	{
		GetMesh()->SetSkeletalMesh(Mesh_2);

		rand = UKismetMathLibrary::RandomIntegerInRange(0, Mesh2_Mats.Num() - 1);
		GetMesh()->SetMaterial(0, Mesh2_Mats[rand]);
	}

	FString RefPath;

	RefPath = "AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Great_Spider/Enemy_SpiderGreater_Animation/ExoGame_Greater_Spider_Charge_RM_Montage.ExoGame_Greater_Spider_Charge_RM_Montage'";
	ChargeAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Heavy, 2.0f, 1.1f, 1.0f, false);

	RefPath = "AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Great_Spider/Enemy_SpiderGreater_Animation/ExoGame_Greater_Spider_Attack_Melee_B_Montage.ExoGame_Greater_Spider_Attack_Melee_B_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Great_Spider/Enemy_SpiderGreater_Animation/ExoGame_Greater_Spider_Attack_Melee_C_Montage.ExoGame_Greater_Spider_Attack_Melee_C_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Great_Spider/Enemy_SpiderGreater_Animation/ExoGame_Greater_Spider_Attack_Melee_Montage.ExoGame_Greater_Spider_Attack_Melee_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.25f, 2.2f, 1.0f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	RightClawDamager = GetWorld()->SpawnActorDeferred<ASword>(ClawDamagerClass, SpawnTrans, this, Instigator);
	RightClawDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(RightClawDamager, SpawnTrans);
	RightClawDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightClawSocket"));

	LeftClawDamager = GetWorld()->SpawnActorDeferred<ASword>(ClawDamagerClass, SpawnTrans, this, Instigator);
	LeftClawDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(LeftClawDamager, SpawnTrans);
	LeftClawDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftClawSocket"));

	WeaponCluster.Add(RightClawDamager);
	WeaponCluster.Add(LeftClawDamager);
}

void AGreaterSpider::ChargeAttack()
{
	Attack(ChargeAttackData);
}

void AGreaterSpider::Crawl_Start()
{
	Play_SoundCue(SoundCue_Ground, true);
	GetWorldTimerManager().SetTimer(CrawlTimer, this, &AGreaterSpider::CrawlFunction, 0.1f, true);
}

void AGreaterSpider::Crawl_End()
{
	SoundActive_Destroy();
	GetWorld()->GetTimerManager().ClearTimer(CrawlTimer);
}

void AGreaterSpider::CrawlFunction()
{
	ImpactFunction(GetMesh()->GetSocketLocation(FName("CrawlSocket")), false);
}

void AGreaterSpider::Impact_Claw()
{
	ImpactFunction(GetMesh()->GetSocketLocation(FName("CrawlSocket")), true);
}

void AGreaterSpider::ImpactFunction(FVector TraceLoc, bool bImpact)
{
	FVector TraceStart = TraceLoc + FVector(0.0f, 0.0f, 67.5f);
	FVector TraceEnd = TraceLoc - FVector(0.0f, 0.0f, 97.5f);

	TArray<FHitResult> HitOuts;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	CollisionQueryParams.bTraceAsyncScene = true;
	CollisionQueryParams.bReturnPhysicalMaterial = true;

	GetWorld()->LineTraceMultiByChannel(HitOuts, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);

	for (auto HitOut : HitOuts)
	{
		if (HitOut.bBlockingHit && HitOut.PhysMaterial->SurfaceType != EPhysicalSurface::SurfaceType8)
		{
			FTransform Trans_PS;
			Trans_PS.SetLocation(HitOut.ImpactNormal * 7.5f + HitOut.ImpactPoint);
			Trans_PS.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
			Trans_PS.SetScale3D(FVector(2.2f, 2.2f, 2.2f));

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Ground_PS, Trans_PS);

			if (bImpact)
			{
				Play_SoundCue(SoundCue_Impact, false);
			}

			break;
		}
	}
}

void AGreaterSpider::ResetDamager_Add()
{
	GetWorld()->GetTimerManager().ClearTimer(CrawlTimer);
}