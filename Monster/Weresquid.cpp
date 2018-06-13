// Fill out your copyright notice in the Description page of Project Settings.

#include "Weresquid.h"
#include "TimerManager.h"
#include "Weapon/Sword.h"
#include "Weapon/HomingProjectile.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Projectile.h"
#include "Miscellaneous/TrailMesh.h"
#include "DrawDebugHelpers.h"

AWeresquid::AWeresquid()
{

}

void AWeresquid::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/Enemy/Weresquid/AnimationRM/WERESQUID_CastSpell4_Anim_RM_Montage.WERESQUID_CastSpell4_Anim_RM_Montage'";
	MagicAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
}

void AWeresquid::Magic_Teleport()
{
	PlayMontage(TeleportMontage, 1.0f, ERestriction::Restriction_Full, RotLerpSpeedNormal);
}

void AWeresquid::Teleport_Notify()
{
	Run_EQS();
}

void AWeresquid::Teleport_Apply()
{
	FHitResult HitOut;
	FVector TraceStart = TeleportLocation + FVector(0.0f, 0.0f, 100.0f);
	FVector TraceEnd = TeleportLocation - FVector(0.0f, 0.0f, 287.5f);

	GetWorld()->LineTraceSingleByChannel(HitOut, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);

	if (HitOut.bBlockingHit)
	{
		FTransform Trail_Trans;
		Trail_Trans.SetLocation(GetMesh()->GetComponentLocation());
		Trail_Trans.SetRotation(FQuat(GetActorRotation()));
		Trail_Trans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		ATrailMesh* TrailMesh = GetWorld()->SpawnActorDeferred<ATrailMesh>(TrailMeshClass, Trail_Trans, this, Instigator);
		UGameplayStatics::FinishSpawningActor(TrailMesh, Trail_Trans);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Teleport_PS, Trail_Trans);
		Play_SoundCue_Location(SoundCue_Teleport, Trail_Trans.GetLocation());

		FRotator Magic_Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Magic_Target->GetActorLocation());
		SetActorRotation(FQuat(FRotator(0.0f, Magic_Rot.Yaw, 0.0f)));

		SetActorLocation(HitOut.ImpactPoint + FVector(0.0f, 0.0f, Cap_Height));
	}
}

void AWeresquid::Run_EQS_Implementation()
{

}

void AWeresquid::Magic_Staff()
{
	Attack(MagicAttackData);
}

void AWeresquid::Projectile_Notify()
{
	projectile_count = 0;
	current_yaw = 0;

	SpawnProjectile();
	Play_SoundCue(SoundCue_Projectile);

	FTransform Trans_PS;
	Trans_PS.SetLocation(GetMesh()->GetSocketLocation("ProjectileSocket"));
	Trans_PS.SetRotation(FQuat());
	Trans_PS.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Projectile_PS, Trans_PS);
}

void AWeresquid::SpawnProjectile()
{
	if (projectile_count < 4)
	{
		FTransform SpawnTrans;

		SpawnTrans.SetLocation(GetMesh()->GetSocketLocation(FName("ProjectileSocket")));
		SpawnTrans.SetRotation(FQuat(FRotator(30.0f, current_yaw, 0.0f)));
		SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		AHomingProjectile* HomingProjectile = GetWorld()->SpawnActorDeferred<AHomingProjectile>(HomingProjectileClass, SpawnTrans, this, Instigator);
		HomingProjectile->MyOwner = this;
		HomingProjectile->DMGType = EDamageType::Damage_Freeze;
		HomingProjectile->AttackData = MagicAttackData;
		HomingProjectile->HomingTarget = Magic_Target;
		UGameplayStatics::FinishSpawningActor(HomingProjectile, SpawnTrans);

		projectile_count += 1;
		current_yaw += 90;

		GetWorldTimerManager().ClearTimer(MagicTimer);
		GetWorldTimerManager().SetTimer(MagicTimer, this, &AWeresquid::SpawnProjectile, 0.1f, false);
	}
}