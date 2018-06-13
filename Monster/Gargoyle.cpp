// Fill out your copyright notice in the Description page of Project Settings.

#include "Gargoyle.h"
#include "Weapon/Sword.h"
#include "TimerManager.h"
#include "Miscellaneous/AttackData.h"
#include "Miscellaneous/IndicatorDecal.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Weapon/Projectile.h"
#include "Controller/MainAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

AGargoyle::AGargoyle()
{
	IndicatorTrans.Init(FTransform(), 9);
	MeteorTimers.Init(FTimerHandle(), 9);
}

void AGargoyle::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/Enemy/Gargoyle/Animation/Gargoyle_Attack1_Montage.Gargoyle_Attack1_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/Enemy/Gargoyle/Animation/Gargoyle_Attack2_Montage.Gargoyle_Attack2_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_AttackWing, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 1.1f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/Gargoyle/Animation/GargoyleHumanoid_FlyAttack1_Fix_Montage.GargoyleHumanoid_FlyAttack1_Fix_Montage'";
	RandomAttackData_Create(RefPath, RandomAttacks_FlyMode, SoundCue_Attack, ERestriction::Restriction_Move, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/Enemy/Gargoyle/Animation/GargoyleHumanoid_FlyAttack2_Fix_Montage.GargoyleHumanoid_FlyAttack2_Fix_Montage'";
	RandomAttackData_Create(RefPath, RandomAttacks_FlyMode, SoundCue_Attack, ERestriction::Restriction_Move, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/Gargoyle/Animation/GargoyleHumanoid_StandCast_Montage.GargoyleHumanoid_StandCast_Montage'";
	FlameAttackData = AttackData_Create(RefPath, SoundCue_FlameAttack, ERestriction::Restriction_Rot, EImpact::Impact_Light, 0.25f, 5.5f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/Gargoyle/Animation/GargoyleHumanoid_FlyCast_Fix_Montage.GargoyleHumanoid_FlyCast_Fix_Montage'";
	FlyCastData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Heavy, 2.0f, 2.2f, 1.0f, false);

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

	RightWingDamager = GetWorld()->SpawnActorDeferred<ASword>(WingDamagerClass, SpawnTrans, this, Instigator);
	RightWingDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(RightWingDamager, SpawnTrans);
	RightWingDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightWingSocket"));
	RightWingDamager->GetSenser()->SetRelativeLocation(RightWingDamager->GetSenser()->GetRelativeTransform().GetLocation() + FVector(0.0f, 12.5f, 0.0f));

	LeftWingDamager = GetWorld()->SpawnActorDeferred<ASword>(WingDamagerClass, SpawnTrans, this, Instigator);
	LeftWingDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(LeftWingDamager, SpawnTrans);
	LeftWingDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftWingSocket"));
	LeftWingDamager->GetSenser()->SetRelativeLocation(LeftWingDamager->GetSenser()->GetRelativeTransform().GetLocation() - FVector(0.0f, 12.5f, 0.0f));

	RightTalonDamager = GetWorld()->SpawnActorDeferred<ASword>(TalonDamagerClass, SpawnTrans, this, Instigator);
	RightTalonDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(RightTalonDamager, SpawnTrans);
	RightTalonDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightTalonSocket"));

	LeftTalonDamager = GetWorld()->SpawnActorDeferred<ASword>(TalonDamagerClass, SpawnTrans, this, Instigator);
	LeftTalonDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(LeftTalonDamager, SpawnTrans);
	LeftTalonDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftTalonSocket"));

	FlameDamager = GetWorld()->SpawnActorDeferred<ASword>(FlameDamagerClass, SpawnTrans, this, Instigator);
	FlameDamager->MyOwner = this;
	FlameDamager->DMGType = EDamageType::Damage_Fire;
	UGameplayStatics::FinishSpawningActor(FlameDamager, SpawnTrans);

	WeaponCluster.Add(FlameDamager);
	WeaponCluster.Add(RightClawDamager);
	WeaponCluster.Add(LeftClawDamager);
	WeaponCluster.Add(RightWingDamager);
	WeaponCluster.Add(LeftWingDamager);
	WeaponCluster.Add(RightTalonDamager);
	WeaponCluster.Add(LeftTalonDamager);

	DeathDatas.Empty();
	DeathDatas.Add(DeathStand_Data);
	HeavyHitDatas.Empty();
	HeavyHitDatas.Add(GetHitStand_Data);
}

void AGargoyle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsDead)
	{
		FVector SocketLoc = GetMesh()->GetSocketLocation("BreatheSocket");

		FlameDamager->GetWeaponTrailPSPoint()->SetWorldLocation(SocketLoc);
		FlameDamager->GetWeaponTrailPSPoint()->SetWorldRotation(GetActorRotation());

		FlameDamager->SetActorLocation(SocketLoc);
		FRotator FD_TargetRot_Interp = UKismetMathLibrary::RInterpTo(FlameDamager->GetActorRotation(), GetActorRotation(), DeltaTime, 2.0f);
		FlameDamager->SetActorRotation(FQuat(FD_TargetRot_Interp));

		if (StandSigil_PS_Comp)
		{
			StandSigil_PS_Comp->SetWorldLocation(SocketLoc);
			StandSigil_PS_Comp->SetWorldRotation(FQuat(GetActorRotation()));
		}
	}
}

void AGargoyle::FlyFunc()
{
	PlayMontage(Fly_Montage, 1.0f, ERestriction::Restriction_Rot, 1.1f);
	DeathDatas.Empty();
	DeathDatas.Add(DeathFly_Data);
	HeavyHitDatas.Empty();
	HeavyHitDatas.Add(GetHitFly_Data);
	RunSpeed = 600.0f;
	WalkSpeed = 600.0f;
	AttackDistance = 137.5f;
	bIsFlyMode = true;
}

void AGargoyle::LandFunc()
{
	PlayMontage(Land_Montage, 1.0f, ERestriction::Restriction_Rot, 1.1f);
	DeathDatas.Empty();
	DeathDatas.Add(DeathStand_Data);
	HeavyHitDatas.Empty();
	HeavyHitDatas.Add(GetHitStand_Data);
	RunSpeed = 300.0f;
	WalkSpeed = 300.0f;
	AttackDistance = 167.5f;
	bIsFlyMode = false;
}

void AGargoyle::RandomAttack()
{
	int rand = 0;

	if (bIsFlyMode)
	{
		rand = UKismetMathLibrary::RandomIntegerInRange(0, RandomAttacks_FlyMode.Num() - 1);

		if (RandomAttacks_FlyMode[rand])
		{
			Attack(RandomAttacks_FlyMode[rand]);
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed / 2.0f;
		}
	}
	else
	{
		rand = UKismetMathLibrary::RandomIntegerInRange(0, RandomAttacks.Num() - 1);

		if (RandomAttacks[rand])
		{
			Attack(RandomAttacks[rand]);
		}
	}
}

void AGargoyle::ResetMontage_Add()
{
	if (bIsFlyMode)
	{
		Walk_Stop();
	}
	else
	{
		Walk_Start();
	}
}

void AGargoyle::FlyMode_Cast()
{
	Attack(FlyCastData);
}

void AGargoyle::StandMode_Cast()
{
	Attack(StandCastData);
}

void AGargoyle::Meteor_Start()
{
	Meteor_Cast();
	Play_SoundCue(SoundCue_FlyCast, true, FName(), 1.75f);

	FlySigil_PS_Comp = UGameplayStatics::SpawnEmitterAttached(FlySigil_PS, GetMesh(), FName("FlySigilSocket"));

	meteor_count = 0;
	meteorCast_count = 0;
	GetWorldTimerManager().SetTimer(MeteorCastTimer, this, &AGargoyle::Meteor_Cast, 0.1f, true);
}

void AGargoyle::Meteor_Cast()
{
	AMainAIController* AIConRef = Cast<AMainAIController>(GetController());

	if (AIConRef)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(AIConRef->GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(AIConRef->EnemyKeyID));

		if (IsTarget)
		{
			int meteor_yaw = UKismetMathLibrary::RandomIntegerInRange(0, 360);
			int rand_distance = UKismetMathLibrary::RandomIntegerInRange(150, 850);

			FVector TraceStart = IsTarget->GetActorLocation() + FQuat(FRotator(0.0f, meteor_yaw, 0.0f)).GetForwardVector() * rand_distance + FVector(0.0f, 0.0f, 750.0f);
			FVector TraceEnd = TraceStart - FVector(0.0f, 0.0f, 1500.0f);
			FHitResult HitOut;

			TArray<AActor*> FoundActors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALibertyPrimeCharacter::StaticClass(), FoundActors);
			TArray<AActor*> FoundWeapons;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALibertyPrimeCharacter::StaticClass(), FoundWeapons);

			FoundActors.Append(FoundWeapons);

			FCollisionQueryParams CollisionQueryParams;
			CollisionQueryParams.AddIgnoredActors(FoundActors);
			CollisionQueryParams.bTraceAsyncScene = true;
			FCollisionResponseParams ResponseParams;
			ResponseParams.CollisionResponse.Visibility;

			GetWorld()->LineTraceSingleByChannel(HitOut, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams, ResponseParams);

			if (HitOut.bBlockingHit)
			{
				FTransform IndicatorTrans_Local;
				IndicatorTrans_Local.SetLocation(HitOut.ImpactPoint + HitOut.ImpactNormal * 12.5f);
				IndicatorTrans_Local.SetRotation(FQuat(FRotator(HitOut.ImpactNormal.Rotation())));
				IndicatorTrans_Local.SetScale3D(FVector(1.3f, 1.3f, 1.3f));
				IndicatorTrans[meteor_count] = IndicatorTrans_Local;

				Cast_Indicator = GetWorld()->SpawnActorDeferred<AIndicatorDecal>(IndicatorClass, IndicatorTrans[meteor_count], this, Instigator);
				UGameplayStatics::FinishSpawningActor(Cast_Indicator, IndicatorTrans[meteor_count]);

				GetWorldTimerManager().ClearTimer(MeteorTimers[meteor_count]);
				GetWorldTimerManager().SetTimer(MeteorTimers[meteor_count], this, &AGargoyle::MeteorFunction, 0.75f, false);
			}

			if (meteor_count == 8)
			{
				GetWorldTimerManager().ClearTimer(MeteorCastTimer);
			}
			else
			{
				meteor_yaw += 45;
				meteor_count += 1;
			}
		}
	}
}

void AGargoyle::MeteorFunction()
{
	if (meteorCast_count <= 8)
	{
		IndicatorTrans[meteorCast_count].SetLocation(IndicatorTrans[meteorCast_count].GetLocation() + FVector(0.0f, 0.0f, 1250.0f));
		IndicatorTrans[meteorCast_count].SetRotation(FQuat(FRotator(-90.0f, 0.0f, 0.0f)));
		IndicatorTrans[meteorCast_count].SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		AProjectile* LocalProjectile = GetWorld()->SpawnActorDeferred<AProjectile>(ProjectileClass, IndicatorTrans[meteorCast_count], this, Instigator);
		LocalProjectile->MyOwner = this;
		LocalProjectile->weapon_type = EWeaponType::Weapon_None;
		LocalProjectile->AttackData = FlyCastData;
		UGameplayStatics::FinishSpawningActor(LocalProjectile, IndicatorTrans[meteorCast_count]);

		meteorCast_count += 1;
	}
}

void AGargoyle::ResetDamager_Add()
{
	GetWorld()->GetTimerManager().ClearTimer(FlameTimer);
	GetWorld()->GetTimerManager().ClearTimer(MeteorCastTimer);
	ActiveSensers_Add(FlameDamager);
	Orb_Destroy();
	Sigil_Destroy();
	StandSigil_Destroy();
}

void AGargoyle::Sigil_Destroy()
{
	if (FlySigil_PS_Comp)
	{
		FlySigil_PS_Comp->DestroyComponent();

		FTransform Trans_PS;
		Trans_PS.SetLocation(GetMesh()->GetSocketLocation(FName("FlySigilSocket")));
		Trans_PS.SetRotation(GetMesh()->GetSocketQuaternion(FName("FlySigilSocket")));
		Trans_PS.SetScale3D(FVector(1.75f, 1.75f, 1.75f));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SigilDestroy_PS, Trans_PS);
	}
}

void AGargoyle::StandSigil_Destroy()
{
	if (StandSigil_PS_Comp)
	{
		StandSigil_PS_Comp->DestroyComponent();

		FTransform Trans_PS;
		Trans_PS.SetLocation(GetMesh()->GetSocketLocation(FName("BreatheSocket")));
		Trans_PS.SetRotation(GetMesh()->GetSocketQuaternion(FName("BreatheSocket")));
		Trans_PS.SetScale3D(FVector(1.75f, 1.75f, 1.75f));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), StandSigilDestroy_PS, Trans_PS);
	}
}

void AGargoyle::Orb_Destroy()
{
	if (Orb_PS_Comp)
	{
		Orb_PS_Comp->DestroyComponent();
	}
}

void AGargoyle::OrbSpawn()
{
	Play_SoundCue(SoundCue_Orb, true);

	Orb_PS_Comp = UGameplayStatics::SpawnEmitterAttached(StandOrb_PS, GetMesh(), FName("OrbSocket"));
}

void AGargoyle::FlameAttack()
{
	Attack(FlameAttackData);
}

void AGargoyle::FlameStart()
{
	FlameDamager->StartTrail();

	Play_SoundCue(SoundCue_FlameAttack, true);

	Orb_Destroy();

	FTransform Trans_PS;
	Trans_PS.SetLocation(GetMesh()->GetSocketLocation(FName("BreatheSocket")));
	Trans_PS.SetRotation(FQuat(GetActorRotation()));
	Trans_PS.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	StandSigil_PS_Comp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), StandSigil_PS, Trans_PS);
	GetWorldTimerManager().SetTimer(FlameTimer, this, &AGargoyle::FlameInit, 0.3f, false);
}

void AGargoyle::FlameEnd()
{
	FlameDamager->EndTrail();
	StandSigil_Destroy();
	GetWorld()->GetTimerManager().ClearTimer(FlameTimer);
	SoundComponents.Add(SoundComponent_Active);
}

void AGargoyle::FlameInit()
{
	ActiveSensers_Add(FlameDamager);
	GetWorldTimerManager().SetTimer(FlameTimer, this, &AGargoyle::FlameFunction, 0.22f, true);
}

void AGargoyle::FlameFunction()
{
	FlameDamager->ResetDamagedChars();
}