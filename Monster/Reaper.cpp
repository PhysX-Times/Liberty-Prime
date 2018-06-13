// Fill out your copyright notice in the Description page of Project Settings.

#include "Reaper.h"
#include "TimerManager.h"
#include "Weapon/Sword.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Engine.h"

AReaper::AReaper()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/HeroicFantasyBossesVol1/Reaper/Animations/REAPER_death_RM_Montage.REAPER_death_RM_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj(TEXT("AnimMontage'/Game/HeroicFantasyBossesVol1/Reaper/Animations/REAPER_getHit_RM_Montage.REAPER_getHit_RM_Montage'"));
	HitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));
	HeavyHitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));

	ProjectilePoint_1 = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectilePoint_1"));
	ProjectilePoint_1->SetupAttachment(RootComponent);
	ProjectilePoint_2 = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectilePoint_2"));
	ProjectilePoint_2->SetupAttachment(RootComponent);
	ProjectilePoint_3 = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectilePoint_3"));
	ProjectilePoint_3->SetupAttachment(RootComponent);
}

void AReaper::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/Reaper/Animations/REAPER_castSpellA_RM_Montage.REAPER_castSpellA_RM_Montage'";
	MagicAttackData_A = AttackData_Create(RefPath, SoundCue_GetHit, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 2.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/Reaper/Animations/REAPER_castSpellB_RM_Montage.REAPER_castSpellB_RM_Montage'";
	MagicAttackData_B = AttackData_Create(RefPath, SoundCue_GetHit, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 2.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/Reaper/Animations/REAPER_scytheAttack1_RM_Montage.REAPER_scytheAttack1_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 0.75f, false);
	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/Reaper/Animations/REAPER_scytheAttack2_RM_Montage.REAPER_scytheAttack2_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 0.75f, false);
	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/Reaper/Animations/REAPER_scytheAttack3_RM_Montage.REAPER_scytheAttack3_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 0.75f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	ScytheDamager = GetWorld()->SpawnActorDeferred<ASword>(ScytheDamagerClass, SpawnTrans, this, Instigator);
	ScytheDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(ScytheDamager, SpawnTrans);
	ScytheDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("ScytheSocket"));

	WeaponCluster.Add(ScytheDamager);

	FStringAssetReference GlitchCurveFloatRef("CurveFloat'/Game/AI/ReaperCurve.ReaperCurve'");
	GlitchCurveFloat = Cast<UCurveFloat>(GlitchCurveFloatRef.TryLoad());
	InitializeFunction(&GlitchLine, FName("GlitchFunction"), GlitchCurveFloat);
}

void AReaper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TL_CheckTick(&GlitchLine, DeltaTime);
}

void AReaper::Spawn_Twin()
{
	Spawn_InitialLoc = GetActorLocation();

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorRightVector() * 125.0f + GetActorLocation());
	SpawnTrans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = Instigator;
	SpawnParams.Owner = this;

	Reaper_Twin = GetWorld()->SpawnActor<AReaper>(ReaperClass, SpawnTrans, SpawnParams);
	Reaper_Twin->GlitchLine.PlayFromStart();
	Reaper_Twin->Health = Health;
	Reaper_Twin->SetActorRotation(FQuat(GetActorRotation()));
	Reaper_Twin->Twin_Play();
	SpawnCount += 1;
	GetWorldTimerManager().SetTimer(GlitchTimer, this, &AReaper::Glitch, 0.05f, false);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Spawn");
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::FromInt(SpawnCount));

	IsInvincible = true;
	Reaper_Twin->IsInvincible = true;
	bFuck = true;
	Play_SoundCue(MagicB_Cue, true);
}

void AReaper::Twin_Reset()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, "Reset");
	bFuck = false;
	IsInvincible = false;
}

void AReaper::Twin_Play()
{
	PlayMontage(MagicAttackB_Montage, 1.0f, ERestriction::Restriction_Full, RotLerpSpeedNormal);
}


void AReaper::Glitch()
{
	if (bFuck)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, "True");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, "False");
	}

	if (bFuck)
	{
		FVector Target_Loc;
		FVector Target_Loc_Twin;

		FVector GlitchOffset = GetActorRightVector() * 125.0f;

		if (bSwitcher)
		{
			Target_Loc = Spawn_InitialLoc + GlitchOffset;
			Target_Loc_Twin = Spawn_InitialLoc - GlitchOffset;
		}
		else
		{
			Target_Loc = Spawn_InitialLoc - GlitchOffset;
			Target_Loc_Twin = Spawn_InitialLoc + GlitchOffset;
		}

		bSwitcher = !bSwitcher;

		SetActorLocation(Spawn_InitialLoc);
		Reaper_Twin->SetActorLocation(Spawn_InitialLoc);

		FHitResult HitOut;

		TArray<AActor*> AllActors;
		AllActors.Add(this);
		AllActors.Add(Reaper_Twin);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActors(AllActors);
		CollisionQueryParams.bTraceAsyncScene = true;

		GetWorld()->LineTraceSingleByChannel(HitOut, Spawn_InitialLoc, Target_Loc, ECollisionChannel::ECC_Visibility, CollisionQueryParams);

		if (HitOut.bBlockingHit)
		{
			Target_Loc = HitOut.ImpactNormal * 12.5f + HitOut.ImpactPoint;
		}

		GetWorld()->LineTraceSingleByChannel(HitOut, Spawn_InitialLoc, Target_Loc_Twin, ECollisionChannel::ECC_Visibility, CollisionQueryParams);

		if (HitOut.bBlockingHit)
		{
			Target_Loc_Twin = HitOut.ImpactNormal * 12.5f + HitOut.ImpactPoint;
		}

		SetActorLocation(Target_Loc);
		Reaper_Twin->SetActorLocation(Target_Loc_Twin);

		GetWorld()->GetTimerManager().ClearTimer(GlitchTimer);
		GetWorldTimerManager().SetTimer(GlitchTimer, this, &AReaper::Glitch, 0.05f, false);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(GlitchTimer);
	}
}

void AReaper::GlitchFunction(float Val)
{
	ApplyFunction(FName("SimDis_dissolve_amount"), Val);
}

void AReaper::MagicAttack_A()
{
	Attack(MagicAttackData_A);
}

void AReaper::MagicAttack_B()
{
	Attack(MagicAttackData_B);
}