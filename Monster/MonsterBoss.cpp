// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterBoss.h"
#include "Weapon/Sword.h"
#include "TimerManager.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/PhysicalMaterials/PhysicalMaterial.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

AMonsterBoss::AMonsterBoss()
{

}

void AMonsterBoss::BeginPlay()
{
	Super::BeginPlay();

	DMGType = EDamageType::Damage_Fire;

	FString RefPath;

	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/MonsterBoss/AnimationRM/BOSS_1_jumpAttack_RM_Montage.BOSS_1_jumpAttack_RM_Montage'";
	JumpAttackData = AttackData_Create(RefPath, SoundCue_GetHit, ERestriction::Restriction_Rot, EImpact::Impact_Heavy, 2.75f, 2.75f, 1.0f, false);

	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/MonsterBoss/AnimationRM/BOSS_1_roarSpreadFire_RM_Montage.BOSS_1_roarSpreadFire_RM_Montage'";
	FlameThrowAttack = AttackData_Create(RefPath, SoundCue_FireBreathe, ERestriction::Restriction_Rot, EImpact::Impact_Light, 0.22f, 4.4f, 1.0f, false);

	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/MonsterBoss/AnimationRM/BOSS_1_attack1_RM_Montage.BOSS_1_attack1_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/MonsterBoss/AnimationRM/BOSS_1_attack2_RM_Montage.BOSS_1_attack2_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/MonsterBoss/AnimationRM/BOSS_1_attack3_RM_Montage.BOSS_1_attack3_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Heavy, 2.0f, 2.2f, 1.0f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	FlameDamager = GetWorld()->SpawnActorDeferred<ASword>(FlameDamagerClass, SpawnTrans, this, Instigator);
	FlameDamager->MyOwner = this;
	FlameDamager->DMGType = EDamageType::Damage_Fire;
	UGameplayStatics::FinishSpawningActor(FlameDamager, SpawnTrans);

	WeaponCluster.Add(FlameDamager);
}

void AMonsterBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsDead)
	{
		FVector SocketLoc = GetMesh()->GetSocketLocation("FlameSocket");
		FRotator FD_TargetRot = GetMesh()->GetSocketRotation("FlameSocket");

		FlameDamager->GetWeaponTrailPSPoint()->SetWorldLocation(SocketLoc);
		FlameDamager->GetWeaponTrailPSPoint()->SetWorldRotation(FQuat(FD_TargetRot));

		FlameDamager->SetActorLocation(SocketLoc);
		FRotator FD_TargetRot_Interp = UKismetMathLibrary::RInterpTo(FlameDamager->GetActorRotation(), FD_TargetRot, DeltaTime, 3.7f);
		FlameDamager->SetActorRotation(FQuat(FD_TargetRot_Interp));
	}

	if (bFireBreathe)
	{
		FQuat SocketRot = GetMesh()->GetSocketQuaternion(FName("FlameSocket"));
		FVector SocketLoc = GetMesh()->GetSocketLocation(FName("FlameSocket"));

		SocketRot.GetForwardVector();
		FVector TraceEnd = SocketLoc + SocketRot.GetForwardVector() * 1750.0f;
		TArray<FHitResult> HitOuts;

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALibertyPrimeCharacter::StaticClass(), FoundActors);

		for (auto FoundActor : FoundActors)
		{
			ALibertyPrimeCharacter* IsLPC = Cast<ALibertyPrimeCharacter>(FoundActor);

			if (IsLPC)
			{
				if (IsLPC->faction != faction)
				{
					FoundActors.Remove(IsLPC);
				}
			}
		}

		TArray<AActor*> FoundWeapons;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeapon::StaticClass(), FoundWeapons);

		FoundActors.Append(FoundWeapons);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActors(FoundActors);
		CollisionQueryParams.bTraceAsyncScene = true;

		GetWorld()->LineTraceMultiByChannel(HitOuts, SocketLoc, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);

		for (auto HitOut : HitOuts)
		{
			ALibertyPrimeCharacter* IsLPC = Cast<ALibertyPrimeCharacter>(HitOut.GetActor());
			AWeapon* IsWeapon = Cast<AWeapon>(HitOut.GetActor());

			if (!IsWeapon && !IsLPC)
			{
				SocketLoc = HitOut.ImpactPoint + HitOut.ImpactNormal * 32.5f;
				TraceEnd = SocketLoc - FVector(0.0f, 0.0f, 37.5f);

				TArray<FHitResult> Sphere_HitOuts;

				FirstPS_End_Comp->SetWorldLocation(SocketLoc);

				GetWorld()->SweepMultiByChannel(Sphere_HitOuts, SocketLoc, TraceEnd, FQuat(), ECollisionChannel::ECC_Destructible, FCollisionShape::MakeSphere(217.5f), CollisionQueryParams);

				for (auto Sphere_HitOut : Sphere_HitOuts)
				{
					IsLPC = Cast<ALibertyPrimeCharacter>(Sphere_HitOut.GetActor());

					if (IsLPC && !IsLPC->IsDead && !IsLPC->IsInvincible)
					{
						bool IsDamaged = false;

						for (auto DamagedChar : DamagedChars)
						{
							if (DamagedChar == IsLPC)
							{
								IsDamaged = true;
								break;
							}
						}

						if (!IsDamaged)
						{
							IsLPC->Damager(BakeDamageData(this, AttackData, DMGType, EWeaponType::Weapon_None), IsLPC->GetMesh()->GetBoneLocation(Sphere_HitOut.BoneName));
							DamagedChars.AddUnique(IsLPC);
						}
					}
				}

				break;
			}
		}
	}
}

void AMonsterBoss::JumpAttack()
{
	Attack(JumpAttackData);
}

void AMonsterBoss::Blast(FName TraceName, float radius, UParticleSystem* Target_PS, USoundCue* Target_Cue, FVector Target_Scale, EWeaponType weapon_type)
{
	FVector TraceStart = GetMesh()->GetSocketLocation(TraceName) + FVector(0.0f, 0.0f, 50.0f);
	FVector TraceEnd = TraceStart - FVector(0.0f, 0.0f, 150.0f);
	TArray<FHitResult> HitOuts;

	TArray<AActor*> FoundActors;
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeapon::StaticClass(), AllActors);
	FoundActors.Append(AllActors);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALibertyPrimeCharacter::StaticClass(), AllActors);
	FoundActors.Append(AllActors);

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActors(FoundActors);
	CollisionQueryParams.bTraceAsyncScene = true;
	CollisionQueryParams.bReturnPhysicalMaterial = true;

	GetWorld()->LineTraceMultiByChannel(HitOuts, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);

	for (auto HitOut : HitOuts)
	{
		if (HitOut.bBlockingHit && HitOut.PhysMaterial->SurfaceType != EPhysicalSurface::SurfaceType8)
		{
			FVector TargetPoint = HitOut.ImpactPoint + HitOut.ImpactNormal * 8.75f;
			MassDamage(radius, AttackData, TargetPoint, weapon_type);

			FTransform PSTrans;
			PSTrans.SetLocation(TargetPoint);
			PSTrans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
			PSTrans.SetScale3D(Target_Scale);

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Target_PS, PSTrans, true);
			Play_SoundCue_Location(Target_Cue, TargetPoint);

			break;
		}
	}
}

void AMonsterBoss::Smash()
{
	DMGType = EDamageType::Damage_Fire;
	Blast(FName("SmashSocket"), 275.0f, Smash_PS, SoundCue_Smash, FVector(1.25f, 1.25f, 1.25f), EWeaponType::Weapon_Sharp);
}

void AMonsterBoss::Claw_Impact(FName BoneName)
{
	DMGType = EDamageType::Damage_None;
	Blast(BoneName, 175.0f, Impact_PS, SoundCue_Impact, FVector(1.0f, 1.0f, 1.0f), EWeaponType::Weapon_Claw);
}

void AMonsterBoss::FlameThrow()
{
	Attack(FlameThrowAttack);
}

void AMonsterBoss::FlameStart()
{
	FlameDamager->StartTrail();
	Play_SoundCue(SoundCue_FireBreathe, true);
	GetWorldTimerManager().SetTimer(FlameTimer, this, &AMonsterBoss::FlameInit, 0.3f, false);

	FTransform PS_Trans;
	PS_Trans.SetLocation(GetActorLocation());
	PS_Trans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
	PS_Trans.SetScale3D(FVector(0.75f, 0.75f, 0.75f));

	DMGType = EDamageType::Damage_Fire;

	FirstPS_End_Comp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FlamePS_End, PS_Trans);
	bFireBreathe = true;
}

void AMonsterBoss::FlameEnd()
{
	FlameDamager->EndTrail();
	GetWorld()->GetTimerManager().ClearTimer(FlameTimer);
	SoundComponents.Add(SoundComponent_Active);

	bFireBreathe = false;
	FirstPS_End_Comp->Deactivate();

	DamagedChars.Empty();
	DMGType = EDamageType::Damage_None;
}

void AMonsterBoss::FlameInit()
{
	ActiveSensers_Add(FlameDamager);
	GetWorldTimerManager().SetTimer(FlameTimer, this, &AMonsterBoss::FlameFunction, 0.22f, true);
}

void AMonsterBoss::FlameFunction()
{
	FlameDamager->ResetDamagedChars();
	DamagedChars.Empty();
}

void AMonsterBoss::ResetDamager_Add()
{
	GetWorld()->GetTimerManager().ClearTimer(FlameTimer);
	ActiveSensers_Add(FlameDamager);

	if (FirstPS_End_Comp)
	{
		FirstPS_End_Comp->Deactivate();
	}
	bFireBreathe = false;

	DamagedChars.Empty();
	DMGType = EDamageType::Damage_None;
}