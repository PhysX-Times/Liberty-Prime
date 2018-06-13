// Fill out your copyright notice in the Description page of Project Settings.

#include "EarthTitan.h"
#include "Weapon/Sword.h"
#include "TimerManager.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

AEarthTitan::AEarthTitan()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/Enemy/Gorosaurus/Animation/GOROSAURUS_Death_RM_Montage.GOROSAURUS_Death_RM_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));
}

void AEarthTitan::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/EarthTitan/AnimationRM/EARTH_TITAN_leftFootStompAttack_RM_Montage.EARTH_TITAN_leftFootStompAttack_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Heavy, 1.75f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/EarthTitan/AnimationRM/EARTH_TITAN_rightFootStompAttack_RM_Montage.EARTH_TITAN_rightFootStompAttack_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Heavy, 1.75f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/EarthTitan/AnimationRM/EARTH_TITAN_lefttHandAttack_RM_Montage.EARTH_TITAN_lefttHandAttack_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/EarthTitan/AnimationRM/EARTH_TITAN_rightHandAttack_RM_Montage.EARTH_TITAN_rightHandAttack_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/EarthTitan/AnimationRM/EARTH_TITAN_handsSmashAttack_RM_Montage.EARTH_TITAN_handsSmashAttack_RM_Montage'";
	SmashAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Heavy, 2.5f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/EarthTitan/AnimationRM/EARTH_TITAN_hands2HitComboForward_RM_Montage.EARTH_TITAN_hands2HitComboForward_RM_Montage'";
	ComboAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

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

void AEarthTitan::SmashAttack()
{
	Attack(SmashAttackData);
}

void AEarthTitan::ComboAttack()
{
	Attack(ComboAttackData);
}

void AEarthTitan::Smash()
{
	Blast(FName("SmashSocket"), 267.5f, SmashAttackData, Smash_PS, FVector(1.1f, 1.1f, 1.1f), SoundCue_Smash);
}

void AEarthTitan::Stomp(FName SocketName)
{
	Blast(SocketName, 187.5f, AttackData, Stomp_PS, FVector(0.875f, 0.875f, 0.875f), SoundCue_Stomp);
}

void AEarthTitan::Blast(FName SocketName, float radius, UAttackData* Target_Data, UParticleSystem* Target_PS, FVector Target_Scale, USoundCue* Target_Cue)
{
	FVector TraceStart = GetMesh()->GetSocketLocation(SocketName) + FVector(0.0f, 0.0f, 50.0f);
	FVector TraceEnd = TraceStart - FVector(0.0f, 0.0f, 150.0f);
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
		FVector TargetPoint = HitOut.ImpactPoint + HitOut.ImpactNormal * 8.75f;
		MassDamage(radius, Target_Data, TargetPoint, EWeaponType::Weapon_None);

		FTransform PSTrans;
		PSTrans.SetLocation(TargetPoint);
		PSTrans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
		PSTrans.SetScale3D(Target_Scale);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Target_PS, PSTrans, true);
		Play_SoundCue_Location(Target_Cue, TargetPoint);
	}
}

void AEarthTitan::ResetDamager_Add()
{
	GetWorld()->GetTimerManager().ClearTimer(CrawlTimer_R);
	GetWorld()->GetTimerManager().ClearTimer(CrawlTimer_L);
}

void AEarthTitan::CrawlStart_R()
{
	Play_SoundCue(SoundCue_Ground, true);
	GetWorldTimerManager().SetTimer(CrawlTimer_R, this, &AEarthTitan::CrawlFunction_R, 0.05f, true);
}

void AEarthTitan::CrawlEnd_R()
{
	SoundActive_Destroy();
	GetWorld()->GetTimerManager().ClearTimer(CrawlTimer_R);
}

void AEarthTitan::CrawlStart_L()
{
	Play_SoundCue(SoundCue_Ground, true);
	GetWorldTimerManager().SetTimer(CrawlTimer_L, this, &AEarthTitan::CrawlFunction_L, 0.05f, true);
}

void AEarthTitan::CrawlEnd_L()
{
	SoundActive_Destroy();
	GetWorld()->GetTimerManager().ClearTimer(CrawlTimer_L);
}

void AEarthTitan::CrawlFunction(FName SocketName)
{
	FVector TraceStart = GetMesh()->GetSocketLocation(SocketName) + FVector(0.0f, 0.0f, 100.0f);
	FVector TraceEnd = TraceStart - FVector(0.0f, 0.0f, 225.0f);
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
		FTransform PSTrans;
		PSTrans.SetLocation(HitOut.ImpactPoint + HitOut.ImpactNormal * 12.5f);
		PSTrans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
		PSTrans.SetScale3D(FVector(2.75f, 2.75f, 2.75f));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Ground_PS, PSTrans, true);
	}
}

void AEarthTitan::CrawlFunction_R()
{
	CrawlFunction(FName("CrawlRightSocket"));
}

void AEarthTitan::CrawlFunction_L()
{
	CrawlFunction(FName("CrawlLeftSocket"));
}