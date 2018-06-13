// Fill out your copyright notice in the Description page of Project Settings.

#include "SnSKnight.h"
#include "TimerManager.h"
#include "Weapon/Sword.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Miscellaneous/AttackData.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Projectile.h"
#include "Engine.h"
#include "DrawDebugHelpers.h"

ASnSKnight::ASnSKnight()
{
	SlashPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("SlashPoint"));
	SlashPoint->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> SpinFirePSObj(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/Fire/combat/P_Moving_Spin_Fire_02.P_Moving_Spin_Fire_02'"));
	SpinFirePS = SpinFirePSObj.Object;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SpinIcePSObj(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/ICE/combat/P_Moving_Spin_Ice_02.P_Moving_Spin_Ice_02'"));
	SpinIcePS = SpinIcePSObj.Object;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SpinPoisonPSObj(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Skill_Whirlwind/P_Whirlwind_Poison_Veng_Typh_01.P_Whirlwind_Poison_Veng_Typh_01'"));
	SpinPoisonPS = SpinPoisonPSObj.Object;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SpinLightningPSObj(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Skill_Whirlwind/P_Whirlwind_Lightning_01.P_Whirlwind_Lightning_01'"));
	SpinLightningPS = SpinLightningPSObj.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> SpinCueElementalObj(TEXT("SoundCue'/Game/SFX/SpinAttack_Sound_Cue_Elemental.SpinAttack_Sound_Cue_Elemental'"));
	SoundCue_SpinElemental = SpinCueElementalObj.Object;
	static ConstructorHelpers::FObjectFinder<USoundCue> SpinCueObj(TEXT("SoundCue'/Game/SFX/SpinAttack_Sound_Cue.SpinAttack_Sound_Cue'"));
	SoundCue_Spin = SpinCueObj.Object;


	static ConstructorHelpers::FObjectFinder<USoundCue> ImpactFireCueObj(TEXT("SoundCue'/Game/SFX/FireBlast1_Cue.FireBlast1_Cue'"));
	SoundCue_ImpactFire = ImpactFireCueObj.Object;
	static ConstructorHelpers::FObjectFinder<USoundCue> ImpactIceCueObj(TEXT("SoundCue'/Game/SFX/IceBlast2_Cue.IceBlast2_Cue'"));
	SoundCue_ImpactIce = ImpactIceCueObj.Object;
	static ConstructorHelpers::FObjectFinder<USoundCue> ImpactLightningCueObj(TEXT("SoundCue'/Game/SFX/ElectricBlast1_Cue.ElectricBlast1_Cue'"));
	SoundCue_ImpactLightning = ImpactLightningCueObj.Object;
	static ConstructorHelpers::FObjectFinder<USoundCue> ImpactPoisonCueObj(TEXT("SoundCue'/Game/SFX/PoisnBlast2_Cue.PoisnBlast2_Cue'"));
	SoundCue_ImpactPoison= ImpactPoisonCueObj.Object;
	static ConstructorHelpers::FObjectFinder<USoundCue> ImpactCueObj(TEXT("SoundCue'/Game/SFX/ImpactAttackWhoosh_Cue.ImpactAttackWhoosh_Cue'"));
	SoundCue_ImpactAttack = ImpactCueObj.Object;
}

void ASnSKnight::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/ModularKnight/Animations/DynamicSwordAnimset/Animations/RootMotion/DashAttackMontage.DashAttackMontage'";
	DashAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Heavy, 1.75f, 4.7f, 1.0f, false);
	
	RefPath = "AnimMontage'/Game/ModularKnight/Animations/DynamicSwordAnimset/Animations/RootMotion/SpinAttackMontage.SpinAttackMontage'";
	SpinAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 4.7f, 1.0f, false);

	RefPath = "AnimMontage'/Game/ModularKnight/Animations/DynamicSwordAnimset/Animations/RootMotion/ImpactAttackMontage.ImpactAttackMontage'";
	ImpactAttackData = AttackData_Create(RefPath, SoundCue_ImpactAttack, ERestriction::Restriction_Rot, EImpact::Impact_Heavy, 2.5f, 4.7f, 1.0f, false);

	RefPath = "AnimMontage'/Game/ModularKnight/Animations/DynamicSwordAnimset/Animations/RootMotion/SlashAttack_Montage.SlashAttack_Montage'";
	EnergyAttackData = AttackData_Create(RefPath, SoundCue_ImpactAttack, ERestriction::Restriction_Rot, EImpact::Impact_Heavy, 2.5f, 4.7f, 1.0f, false);

	RefPath = "AnimMontage'/Game/ModularKnight/Animations/SnSAnimsetPro/Animations/RootMotion/AttackHeavyCLFast.AttackHeavyCLFast'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.66f, 3.3f, 0.75f, false);

	RefPath = "AnimMontage'/Game/ModularKnight/Animations/SnSAnimsetPro/Animations/RootMotion/AttackLLFast.AttackLLFast'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.22f, 3.3f, 0.75f, false);

	RefPath = "AnimMontage'/Game/ModularKnight/Animations/SnSAnimsetPro/Animations/RootMotion/AttackLD2Fast.AttackLD2Fast'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.22f, 3.3f, 0.75f, false);

	RefPath = "AnimMontage'/Game/ModularKnight/Animations/SnSAnimsetPro/Animations/RootMotion/AttackLd3Fast.AttackLd3Fast'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.11f, 3.3f, 0.75f, false);

	RefPath = "AnimMontage'/Game/ModularKnight/Animations/SnSAnimsetPro/Animations/RootMotion/AttackLdFast.AttackLdFast'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.33f, 3.3f, 0.75f, false);

	RefPath = "AnimMontage'/Game/ModularKnight/Animations/SnSAnimsetPro/Animations/RootMotion/AttackRWhirlwindFast.AttackRWhirlwindFast'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.55f, 3.3f, 0.75f, false);
}

void ASnSKnight::DashAttack()
{
	if (CheckRestriction())
	{
		Attack(DashAttackData);
	}
}

void ASnSKnight::SpinAttack()
{
	if (CheckRestriction())
	{
		Attack(SpinAttackData);
	}
}

void ASnSKnight::EnergyAttack()
{
	if (CheckRestriction())
	{
		Attack(EnergyAttackData);
	}
}

void ASnSKnight::EnergySlash_Generate()
{
	FTransform SpawnTrans;
	SpawnTrans.SetLocation(SlashPoint->GetComponentLocation());
	SpawnTrans.SetRotation(FQuat(FRotator(0.0f, GetActorRotation().Yaw, 0.0f)));
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
	AProjectile* LocalProjectile;

	switch (MySword->DMGType)
	{
	case EDamageType::Damage_Fire:
		LocalProjectile = GetWorld()->SpawnActorDeferred<AProjectile>(Projectile_Fire_Class, SpawnTrans, this, Instigator);
		LocalProjectile->MyOwner = this;
		LocalProjectile->weapon_type = EWeaponType::Weapon_Sharp;
		LocalProjectile->DMGType = MySword->DMGType;
		LocalProjectile->AttackData = EnergyAttackData;
		UGameplayStatics::FinishSpawningActor(LocalProjectile, SpawnTrans);
		Play_SoundCue(SoundCue_ImpactFire, false);
		break;
	case EDamageType::Damage_Freeze:
		LocalProjectile = GetWorld()->SpawnActorDeferred<AProjectile>(Projectile_Ice_Class, SpawnTrans, this, Instigator);
		LocalProjectile->MyOwner = this;
		LocalProjectile->weapon_type = EWeaponType::Weapon_Sharp;
		LocalProjectile->DMGType = MySword->DMGType;
		LocalProjectile->AttackData = EnergyAttackData;
		UGameplayStatics::FinishSpawningActor(LocalProjectile, SpawnTrans);
		Play_SoundCue(SoundCue_ImpactIce, false);
		break;
	case EDamageType::Damage_Poison:
		LocalProjectile = GetWorld()->SpawnActorDeferred<AProjectile>(Projectile_Poison_Class, SpawnTrans, this, Instigator);
		LocalProjectile->MyOwner = this;
		LocalProjectile->weapon_type = EWeaponType::Weapon_Sharp;
		LocalProjectile->DMGType = MySword->DMGType;
		LocalProjectile->AttackData = EnergyAttackData;
		UGameplayStatics::FinishSpawningActor(LocalProjectile, SpawnTrans);
		Play_SoundCue(SoundCue_ImpactPoison, false);
		break;
	case EDamageType::Damage_Lightning:
		LocalProjectile = GetWorld()->SpawnActorDeferred<AProjectile>(Projectile_Lightning_Class, SpawnTrans, this, Instigator);
		LocalProjectile->MyOwner = this;
		LocalProjectile->weapon_type = EWeaponType::Weapon_Sharp;
		LocalProjectile->DMGType = MySword->DMGType;
		LocalProjectile->AttackData = EnergyAttackData;
		UGameplayStatics::FinishSpawningActor(LocalProjectile, SpawnTrans);
		Play_SoundCue(SoundCue_ImpactLightning, false);
		break;
	default:
		break;
	}
}

void ASnSKnight::ImpactAttack()
{
	/*
	if (CheckRestriction())
	{
		Attack(ImpactAttackData);
		BlastActors.Empty();
		ImpactBlast_Count = 0;
	}
	*/
}

void ASnSKnight::ImpactBlast()
{
	/*
	if (ImpactBlast_Count != 7)
	{
		if (ImpactBlast_Count == 0)
		{
			ImpactInit_Vector = GetActorLocation();
			ImpactForward_Vector = GetActorForwardVector();
		}

		ImpactInit_Vector = ImpactInit_Vector + ImpactForward_Vector * 175.0f;
		ImpactBlast_Count += 1;


		FHitResult HitOut;
		FVector TraceStart = ImpactInit_Vector + FVector(0.0f, 0.0f, 325.0f);
		FVector TraceEnd = ImpactInit_Vector - FVector(0.0f, 0.0f, 325.0f);
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
		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, true, 3.5f, 0, 17.5f);
		
		if (HitOut.bBlockingHit)
		{
			FTransform BlastTrans;
			BlastTrans.SetLocation(HitOut.ImpactPoint + HitOut.ImpactNormal * 97.5f);
			BlastTrans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
			BlastTrans.SetScale3D(FVector(1.25f, 1.25f, 1.25f));

			//DrawDebugSphere(GetWorld(), HitOut.ImpactPoint, 200.0f, 22, FColor::Red, true, 3.5f, 0, 12.5f);

			TArray<FHitResult> HitOuts;
			FVector TraceStart = ImpactInit_Vector;
			FVector TraceEnd = TraceStart + FVector(0.0f, 0.0f, 15.0f);

			for (auto AllActor : AllActors)
			{
				ALibertyPrimeCharacter* IsLPC = Cast<ALibertyPrimeCharacter>(AllActor);

				if (IsLPC)
				{
					if (!IsLPC->IsDead && !IsLPC->IsInvincible && IsLPC->faction != faction)
					{
						AllActors.Remove(IsLPC);
					}
				}
			}

			FCollisionObjectQueryParams ObjectQueryParams;
			ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
			FCollisionQueryParams CollisionQueryParams;
			CollisionQueryParams.AddIgnoredActors(AllActors);
			CollisionQueryParams.bTraceAsyncScene = true;

			GetWorld()->SweepMultiByObjectType(HitOuts, TraceStart, TraceEnd, FQuat(), ObjectQueryParams, FCollisionShape::MakeSphere(200.0f), CollisionQueryParams);

			for (auto HitOut : HitOuts)
			{
				ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(HitOut.GetActor());

				if (IsTarget)
				{
					bool IsFound = false;

					for (auto DamagedChar : BlastActors)
					{
						if (IsTarget == DamagedChar)
						{
							IsFound = true;
						}
					}

					if (!IsFound)
					{
						IsTarget->Damager(BakeDamageData(this, ImpactAttackData, DMGType, EWeaponType::Weapon_None), IsTarget->GetActorLocation());
						BlastActors.AddUnique(IsTarget);
					}
				}
			}

			switch (DMGType)
			{
			case EDamageType::Damage_Fire:
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactFirePS, BlastTrans, true);
				Play_SoundCue(SoundCue_ImpactFire, false);
				break;
			case EDamageType::Damage_Freeze:
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactIcePS, BlastTrans, true);
				Play_SoundCue(SoundCue_ImpactIce, true);
				break;
			case EDamageType::Damage_Lightning:
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactLightningPS, BlastTrans, true);
				Play_SoundCue(SoundCue_ImpactLightning, true);
				break;
			case EDamageType::Damage_Poison:
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactPoisonPS, BlastTrans, true);
				Play_SoundCue(SoundCue_ImpactPoison, true);
				break;
			default:
				//Play_SoundCue(SoundCue_Spin, true);
				break;
			}
		}

		GetWorldTimerManager().SetTimer(ImpactTimer, this, &ASnSKnight::ImpactBlast, 0.1f, false);
	}
	*/
}

void ASnSKnight::SpinReset()
{
	MySword->ResetDamagedChars();
}

void ASnSKnight::SpinStart()
{
	IsSpinning = true;
	IsInvincible = true;
	ActiveSensers.AddUnique(MySword);
	GetWorldTimerManager().SetTimer(SpinTimer, this, &ASnSKnight::SpinReset, 0.2f, true);

	FTransform SpinPSTrans;
	SpinPSTrans.SetLocation(GetActorLocation());
	SpinPSTrans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
	SpinPSTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	switch (DMGType)
	{
		case EDamageType::Damage_Fire:
			SpinPS = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpinFirePS, SpinPSTrans, true);
			Play_SoundCue(SoundCue_SpinElemental, true);
			break;
		case EDamageType::Damage_Freeze:
			SpinPS = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpinIcePS, SpinPSTrans, true);
			Play_SoundCue(SoundCue_SpinElemental, true);
			break;
		case EDamageType::Damage_Lightning:
			SpinPS = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpinLightningPS, SpinPSTrans, true);
			Play_SoundCue(SoundCue_SpinElemental, true);
			break;
		case EDamageType::Damage_Poison:
			SpinPS = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpinPoisonPS, SpinPSTrans, true);
			Play_SoundCue(SoundCue_SpinElemental, true);
			break;
		default:
			Play_SoundCue(SoundCue_Spin, true);
			break;
	}

	ParticleCluster.AddUnique(SpinPS);
}

void ASnSKnight::SpinEnd()
{
	IsSpinning = false;
	ResetDamager();

	if (SpinPS)
	{
		SpinPS->Deactivate();
	}

	SoundActive_Destroy();

	GetWorldTimerManager().ClearTimer(SpinTimer);
	ParticleCluster.Remove(SpinPS);
}

void ASnSKnight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsSpinning && SpinPS)
	{
		SpinPS->SetWorldLocation(GetMesh()->GetSocketLocation(FName("SpinEmitterSocket")));
	}
}