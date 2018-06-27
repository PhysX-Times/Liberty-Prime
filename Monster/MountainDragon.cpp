// Fill out your copyright notice in the Description page of Project Settings.

#include "MountainDragon.h"
#include "Weapon/Sword.h"
#include "TimerManager.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/HomingProjectile.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Engine.h"

AMountainDragon::AMountainDragon()
{
	RotLerpSpeed = 2.2f;
	MoveTo_Distance_Add = 325.0f;
}

void AMountainDragon::Tick(float DeltaTime)
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
		FVector TraceEnd = SocketLoc + SocketRot.GetForwardVector() * 1250.0f;
		FHitResult HitOut;

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALibertyPrimeCharacter::StaticClass(), FoundActors);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.bTraceAsyncScene = true;
		CollisionQueryParams.AddIgnoredActors(FoundActors);

		GetWorld()->LineTraceSingleByChannel(HitOut, SocketLoc, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);

		if (HitOut.bBlockingHit)
		{
			ALibertyPrimeCharacter* IsLPC = Cast<ALibertyPrimeCharacter>(HitOut.GetActor());

			if (!IsLPC)
			{
				SocketLoc = HitOut.ImpactPoint + HitOut.ImpactNormal * 32.5f;
				TraceEnd = SocketLoc - FVector(0.0f, 0.0f, 37.5f);

				TArray<FHitResult> Sphere_HitOuts;

				if (FirstPS_End_Comp)
				{
					if (!FirstPS_End_Comp->IsActive())
					{
						FTransform PS_Trans;
						PS_Trans.SetLocation(SocketLoc);
						PS_Trans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
						PS_Trans.SetScale3D(FVector(0.75f, 0.75f, 0.75f));

						FirstPS_End_Comp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FlamePS_End, PS_Trans);
					}

					FirstPS_End_Comp->SetWorldLocation(SocketLoc);
				}
				else
				{
					FTransform PS_Trans;
					PS_Trans.SetLocation(SocketLoc);
					PS_Trans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
					PS_Trans.SetScale3D(FVector(0.75f, 0.75f, 0.75f));

					FirstPS_End_Comp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FlamePS_End, PS_Trans);
				}

				GetWorld()->SweepMultiByChannel(Sphere_HitOuts, SocketLoc, TraceEnd, FQuat(), ECollisionChannel::ECC_Destructible, FCollisionShape::MakeSphere(217.5f), CollisionQueryParams);

				for (auto Sphere_HitOut : Sphere_HitOuts)
				{
					IsLPC = Cast<ALibertyPrimeCharacter>(Sphere_HitOut.GetActor());

					if (IsLPC && !IsLPC->IsDead && !IsLPC->IsInvincible && faction != IsLPC->faction)
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
			}
		}
		else
		{
			if (FirstPS_End_Comp)
			{
				FirstPS_End_Comp->Deactivate();
			}
		}
	}

}

void AMountainDragon::BeginPlay()
{
	Super::BeginPlay();

	DMGType = EDamageType::Damage_Fire;

	FString RefPath;

	RefPath = "AnimMontage'/Game/Enemy/MountainDragon/Animation/MOUNTAIN_DRAGON_FlySpitFire_Montage.MOUNTAIN_DRAGON_FlySpitFire_Montage'";
	FireBallAttack = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Heavy, 2.0f, RotLerpSpeed, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/MountainDragon/Animation/MOUNTAIN_DRAGON_FlySpreadFire_Montage.MOUNTAIN_DRAGON_FlySpreadFire_Montage'";
	FlameThrowAttack = AttackData_Create(RefPath, SoundCue_FireBreathe, ERestriction::Restriction_Rot, EImpact::Impact_Light, 0.25f, RotLerpSpeed, 1.0f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	FlameDamager = GetWorld()->SpawnActorDeferred<ASword>(FlameDamagerClass, SpawnTrans, this, Instigator);
	FlameDamager->MyOwner = this;
	FlameDamager->DMGType = EDamageType::Damage_Fire;
	UGameplayStatics::FinishSpawningActor(FlameDamager, SpawnTrans);

	WeaponCluster.Add(FlameDamager);

	for (int i = 3; i < 14; i++)
	{
		FName target_name = FName(*(FString("MOUNTAIN_DRAGON_-Tail") + FString::FromInt(i)));
		FBodyInstance* body_instance = GetMesh()->GetBodyInstance(target_name);

		if (body_instance)
		{
			body_instance->SetInstanceSimulatePhysics(true);
		}
	}
}

void AMountainDragon::FireBall_Notify()
{
	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetMesh()->GetSocketLocation(FName("FlameSocket")));
	SpawnTrans.SetRotation(GetMesh()->GetSocketQuaternion(FName("FlameSocket")));
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	AHomingProjectile* FireBall = GetWorld()->SpawnActorDeferred<AHomingProjectile>(HomingProjectileClass, SpawnTrans, this, Instigator);
	FireBall->MyOwner = this;
	FireBall->DMGType = EDamageType::Damage_Fire;
	FireBall->AttackData = FireBallAttack;
	UGameplayStatics::FinishSpawningActor(FireBall, SpawnTrans);
}

void AMountainDragon::FlameThrow()
{
	Attack(FlameThrowAttack);
}

void AMountainDragon::FireBallThrow()
{
	Attack(FireBallAttack);
}

void AMountainDragon::FlameStart()
{
	FlameDamager->StartTrail();
	GetWorldTimerManager().SetTimer(FlameTimer, this, &AMountainDragon::FlameInit, 0.3f, false);

	FTransform PS_Trans;
	PS_Trans.SetLocation(GetActorLocation());
	PS_Trans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
	PS_Trans.SetScale3D(FVector(0.75f, 0.75f, 0.75f));

	FirstPS_End_Comp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FlamePS_End, PS_Trans);
	bFireBreathe = true;
}

void AMountainDragon::FlameEnd()
{
	FlameDamager->EndTrail();
	GetWorld()->GetTimerManager().ClearTimer(FlameTimer);
	SoundComponents.Add(SoundComponent_Active);

	bFireBreathe = false;
	FirstPS_End_Comp->Deactivate();

	DamagedChars.Empty();
}

void AMountainDragon::FlameInit()
{
	ActiveSensers_Add(FlameDamager);
	GetWorldTimerManager().SetTimer(FlameTimer, this, &AMountainDragon::FlameFunction, 0.22f, true);
}

void AMountainDragon::FlameFunction()
{
	FlameDamager->ResetDamagedChars();
	DamagedChars.Empty();
}

void AMountainDragon::ResetDamager_Add()
{
	GetWorld()->GetTimerManager().ClearTimer(FlameTimer);
	ActiveSensers_Add(FlameDamager);

	if (FirstPS_End_Comp)
	{
		FirstPS_End_Comp->Deactivate();
	}

	bFireBreathe = false;
	bFocus = true;

	DamagedChars.Empty();
}

void AMountainDragon::PlayMontage_Add()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
	Walk_Start();
}

void AMountainDragon::ResetMontage_Add()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 60.0f, 0.0f);
	Walk_Stop();
}