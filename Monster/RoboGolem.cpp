// Fill out your copyright notice in the Description page of Project Settings.

#include "RoboGolem.h"
#include "Weapon/Sword.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

ARoboGolem::ARoboGolem()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Golem/Enemy_Golem_Animations/ExoGame_Fire_Golem_Death_A_Montage.ExoGame_Fire_Golem_Death_A_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj(TEXT("AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Golem/Enemy_Golem_Animations/ExoGame_Fire_Golem_React_Knockback_RM_Montage.ExoGame_Fire_Golem_React_Knockback_RM_Montage'"));
	HitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));
	HeavyHitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));
}

void ARoboGolem::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/Reaper/Animations/REAPER_castSpellA_RM_Montage.REAPER_castSpellA_RM_Montage'";
	MagicAttackData_A = AttackData_Create(RefPath, SoundCue_GetHit, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 2.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/Reaper/Animations/REAPER_castSpellB_RM_Montage.REAPER_castSpellB_RM_Montage'";
	MagicAttackData_B = AttackData_Create(RefPath, SoundCue_GetHit, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 2.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Golem/Enemy_Golem_Animations/ExoGame_Fire_Golem_Attack_Melee_Montage.ExoGame_Fire_Golem_Attack_Melee_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Heavy, 1.75f, 2.2f, 0.75f, false);
	RefPath = "AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Golem/Enemy_Golem_Animations/ExoGame_Fire_Golem_Attack_MeleeB_Montage.ExoGame_Fire_Golem_Attack_MeleeB_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 0.75f, false);
	RefPath = "AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Golem/Enemy_Golem_Animations/ExoGame_Fire_Golem_Attack_MeleeC_Montage.ExoGame_Fire_Golem_Attack_MeleeC_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Heavy, 2.0f, 2.2f, 0.75f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	HandDamager = GetWorld()->SpawnActorDeferred<ASword>(HandDamagerClass, SpawnTrans, this, Instigator);
	HandDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(HandDamager, SpawnTrans);
	HandDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("HandSocket"));

	WeaponCluster.Add(HandDamager);
}

void ARoboGolem::Roll_Start()
{
	PlayMontage(RollStart_Montage, 1.0f, ERestriction::Restriction_Rot, 3.3f);
	GetCharacterMovement()->MaxWalkSpeed = 700.0f;
	CanBeHeavyHit = false;
	bRolePose = true;
}

void ARoboGolem::Roll_End()
{
	PlayMontage(RollEnd_Montage, 1.0f, ERestriction::Restriction_Full, 3.3f);
	GetCharacterMovement()->MaxWalkSpeed = 175.0f;
	CanBeHeavyHit = true;
	bRolePose = false;
	SoundActive_Destroy();
}

void ARoboGolem::HandSmash()
{
	DMGType = EDamageType::Damage_Fire;
	Smash(FName("HandSocket"), 200.0f, FVector(0.75f, 0.75f, 0.75f), HandSmashPS, HandSmashCue);
	DMGType = EDamageType::Damage_None;
}

void ARoboGolem::LegSmash()
{
	Smash(FName("SmashLegSocket"), 210.0f, FVector(1.0f, 1.0f, 1.0f), LegSmashPS, LegSmashCue);
}

void ARoboGolem::Smash(FName SocketName, float radius, FVector PS_Scale, UParticleSystem* SmashPS, USoundCue* SmashCue)
{
	FVector TraceStart = GetMesh()->GetSocketLocation(SocketName) + FVector(0.0f, 0.0f, 50.0f);
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
		MassDamage(radius, AttackData, TargetPoint, EWeaponType::Weapon_None);

		FTransform PSTrans;
		PSTrans.SetLocation(TargetPoint);
		PSTrans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
		PSTrans.SetScale3D(PS_Scale);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SmashPS, PSTrans, true);
		Play_SoundCue_Location(SmashCue, TargetPoint);
	}
}