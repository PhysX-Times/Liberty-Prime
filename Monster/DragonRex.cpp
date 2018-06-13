// Fill out your copyright notice in the Description page of Project Settings.

#include "DragonRex.h"
#include "Weapon/Sword.h"
#include "TimerManager.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Projectile.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Engine.h"

ADragonRex::ADragonRex()
{

}

void ADragonRex::Tick(float DeltaTime)
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

void ADragonRex::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/DragonRexAlpha/AnimationRM/DRAGON_REX_ALPHA_bite_RM_Montage.DRAGON_REX_ALPHA_bite_RM_Montage'";
	BiteAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/DragonRexAlpha/AnimationRM/DRAGON_REX_ALPHA_tailAttack_RM_Montage.DRAGON_REX_ALPHA_tailAttack_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.5f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/DragonRexAlpha/AnimationRM/DRAGON_REX_ALPHA_spreadFireRoar_RM_Montage.DRAGON_REX_ALPHA_spreadFireRoar_RM_Montage'";
	FlameThrowAttack = AttackData_Create(RefPath, SoundCue_FireBreathe, ERestriction::Restriction_Rot, EImpact::Impact_Light, 0.22f, 3.75f, 1.0f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	TailDamager = GetWorld()->SpawnActorDeferred<ASword>(TailDamagerClass, SpawnTrans, this, Instigator);
	TailDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(TailDamager, SpawnTrans);
	TailDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("TailSocket"));

	BiteDamager = GetWorld()->SpawnActorDeferred<ASword>(BiteDamagerClass, SpawnTrans, this, Instigator);
	BiteDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(BiteDamager, SpawnTrans);
	BiteDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("BiteSocket"));

	FlameDamager = GetWorld()->SpawnActorDeferred<ASword>(FlameDamagerClass, SpawnTrans, this, Instigator);
	FlameDamager->MyOwner = this;
	FlameDamager->DMGType = EDamageType::Damage_Fire;
	UGameplayStatics::FinishSpawningActor(FlameDamager, SpawnTrans);

	WeaponCluster.Add(FlameDamager);
	WeaponCluster.Add(BiteDamager);
	WeaponCluster.Add(TailDamager);

}

void ADragonRex::FlameThrow()
{
	Attack(FlameThrowAttack);
}

void ADragonRex::BiteAttack()
{
	Attack(BiteAttackData);
}

void ADragonRex::FlameStart()
{
	FlameDamager->StartTrail();
	GetWorldTimerManager().SetTimer(FlameTimer, this, &ADragonRex::FlameInit, 0.3f, false);

	FTransform PS_Trans;
	PS_Trans.SetLocation(GetActorLocation());
	PS_Trans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
	PS_Trans.SetScale3D(FVector(0.75f, 0.75f, 0.75f));

	DMGType = EDamageType::Damage_Fire;

	FirstPS_End_Comp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FlamePS_End, PS_Trans);
	bFireBreathe = true;
}

void ADragonRex::FlameEnd()
{
	FlameDamager->EndTrail();
	GetWorld()->GetTimerManager().ClearTimer(FlameTimer);
	SoundComponents.Add(SoundComponent_Active);

	bFireBreathe = false;
	FirstPS_End_Comp->Deactivate();

	DamagedChars.Empty();
	DMGType = EDamageType::Damage_None;
}

void ADragonRex::FlameInit()
{
	ActiveSensers_Add(FlameDamager);
	GetWorldTimerManager().SetTimer(FlameTimer, this, &ADragonRex::FlameFunction, 0.22f, true);
}

void ADragonRex::FlameFunction()
{
	FlameDamager->ResetDamagedChars();
	DamagedChars.Empty();
}

void ADragonRex::ResetDamager_Add()
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