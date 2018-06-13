// Fill out your copyright notice in the Description page of Project Settings.

#include "GiantTroll.h"
#include "Weapon/Sword.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

AGiantTroll::AGiantTroll()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/Enemy/Troll/Animation/TROLL_Death_RM_Montage.TROLL_Death_RM_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj(TEXT("AnimMontage'/Game/Enemy/Troll/Animation/TROLL_GetHit_RM_Montage.TROLL_GetHit_RM_Montage'"));
	HitDatas.Add(FMontageData(HitMontageObj.Object, 0.75f));
	HeavyHitDatas.Add(FMontageData(HitMontageObj.Object, 0.75f));
}

void AGiantTroll::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/Enemy/Troll/Animation/TROLL_2HitCombo_RM_Montage.TROLL_2HitCombo_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/Troll/Animation/TROLL_SmashAttack_RM_Montage.TROLL_SmashAttack_RM_Montage'";
	SmashAttackData = AttackData_Create(RefPath, SoundCue_GetHit, ERestriction::Restriction_Rot, EImpact::Impact_Heavy, 2.5f, 2.2f, 1.0f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	LeftHandDamager = GetWorld()->SpawnActorDeferred<ASword>(HandDamagerClass, SpawnTrans, this, Instigator);
	LeftHandDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(LeftHandDamager, SpawnTrans);
	LeftHandDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftHandSocket"));

	RightHandDamager = GetWorld()->SpawnActorDeferred<ASword>(HandDamagerClass, SpawnTrans, this, Instigator);
	RightHandDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(RightHandDamager, SpawnTrans);
	RightHandDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightHandSocket"));

	WeaponCluster.Add(LeftHandDamager);
	WeaponCluster.Add(RightHandDamager);
}

void AGiantTroll::SmashAttack()
{
	Attack(SmashAttackData);
}

void AGiantTroll::SmashBlast()
{
	FVector TraceStart = GetMesh()->GetSocketLocation("BlastSocket") + FVector(0.0f, 0.0f, 50.0f);
	FVector TraceEnd = TraceStart - FVector(0.0f, 0.0f, 175.5f);
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
		FVector TargetPoint = HitOut.ImpactPoint + HitOut.ImpactNormal * 12.5f;
		MassDamage(175.0f, SmashAttackData, TargetPoint, EWeaponType::Weapon_None);

		FTransform PSTrans;
		PSTrans.SetLocation(TargetPoint);
		PSTrans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
		PSTrans.SetScale3D(FVector(2.0f, 2.0f, 2.0f));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SmashPS, PSTrans, true);
		Play_SoundCue_Location(SmashCue, TargetPoint);
	}
}