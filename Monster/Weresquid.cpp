// Fill out your copyright notice in the Description page of Project Settings.

#include "Weresquid.h"
#include "TimerManager.h"
#include "Weapon/HomingProjectile.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Projectile.h"
#include "Miscellaneous/TrailMesh.h"
#include "Miscellaneous/SummonCaster.h"

AWeresquid::AWeresquid()
{

}

void AWeresquid::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/Enemy/Weresquid/AnimationRM/WERESQUID_CastSpell4_Anim_RM_Montage.WERESQUID_CastSpell4_Anim_RM_Montage'";
	MagicAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/Weresquid/AnimationRM/WERESQUID_CastSpell2_Anim_RM_Montage.WERESQUID_CastSpell2_Anim_RM_Montage'";
	SummonAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Heavy, 2.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/Weresquid/AnimationRM/WERESQUID_CastSpell1_Anim_RM_Montage.WERESQUID_CastSpell1_Anim_RM_Montage'";
	ArrowAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Light, 0.25f, 2.2f, 1.0f, false);

}

void AWeresquid::Tick(float DeltaTime)
{
	if (Magic_Target && SigilPS_Comp)
	{
		FRotator Comp_Rot = UKismetMathLibrary::FindLookAtRotation(SigilPS_Comp->GetComponentLocation(), Magic_Target->GetActorLocation());
		FRotator Interp_Rot = UKismetMathLibrary::RInterpTo(SigilPS_Comp->GetComponentRotation(), Comp_Rot, DeltaTime, 4.4f);
		SigilPS_Comp->SetWorldRotation(FQuat(Interp_Rot));
	}
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

void AWeresquid::Magic_Arrow()
{
	Attack(ArrowAttackData);
}

void AWeresquid::Magic_Summon()
{
	Attack(SummonAttackData);

	summon_yaw = UKismetMathLibrary::RandomIntegerInRange(0, 360);
	summon_count = 0;
}

void AWeresquid::Projectile_Notify()
{
	projectile_count = 0;
	current_yaw = UKismetMathLibrary::RandomIntegerInRange(0, 360);

	SpawnProjectile();
}

void AWeresquid::Arrow_Notify()
{
	arrow_count = 0;

	Sigil_Destory();
	GetWorldTimerManager().ClearTimer(ArrowTimer);

	FTransform Trans_PS;
	Trans_PS.SetLocation(GetMesh()->GetSocketLocation("ArrowSocket"));

	FRotator Magic_Rot = UKismetMathLibrary::FindLookAtRotation(Trans_PS.GetLocation(), Magic_Target->GetActorLocation());

	Trans_PS.SetRotation(FQuat(Magic_Rot));
	Trans_PS.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	SigilPS_Comp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Sigil_PS, Trans_PS);

	SpawnArrow();
}

void AWeresquid::SpawnProjectile()
{
	if (projectile_count < 4)
	{
		Play_SoundCue(SoundCue_Projectile);

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

void AWeresquid::SpawnArrow()
{
	if (arrow_count < 111)
	{
		FTransform SpawnTrans;

		float rand_x = UKismetMathLibrary::RandomFloatInRange(-100.0f, 100.0f);
		float rand_y = UKismetMathLibrary::RandomFloatInRange(-100.0f, 100.0f);
		
		SpawnTrans.SetLocation(SigilPS_Comp->GetComponentLocation() + SigilPS_Comp->GetUpVector() * rand_x + GetActorRightVector() * rand_y);
		SpawnTrans.SetRotation(SigilPS_Comp->GetComponentQuat());
		SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		AHomingProjectile* ArrowProjectile = GetWorld()->SpawnActorDeferred<AHomingProjectile>(ArrowProjectileClass, SpawnTrans, this, Instigator);
		ArrowProjectile->MyOwner = this;
		ArrowProjectile->DMGType = EDamageType::Damage_Poison;
		ArrowProjectile->AttackData = ArrowAttackData;
		ArrowProjectile->bArrow = true;
		UGameplayStatics::FinishSpawningActor(ArrowProjectile, SpawnTrans);

		arrow_count += 1;

		GetWorldTimerManager().ClearTimer(ArrowTimer);
		GetWorldTimerManager().SetTimer(ArrowTimer, this, &AWeresquid::SpawnArrow, 0.02f, false);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(ArrowTimer);
		Sigil_Destory();
	}
}

void AWeresquid::ResetDamager_Add()
{
	GetWorld()->GetTimerManager().ClearTimer(MagicTimer);
}

void AWeresquid::Die_Add()
{
	Super::Die_Add();

	GetWorld()->GetTimerManager().ClearTimer(ArrowTimer);
	Sigil_Destory();
}

void AWeresquid::Sigil_Destory()
{
	if (SigilPS_Comp)
	{
		FTransform Trans_PS;

		Trans_PS.SetLocation(SigilPS_Comp->GetComponentLocation());
		Trans_PS.SetRotation(SigilPS_Comp->GetComponentQuat());
		Trans_PS.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Sigil_End_PS, Trans_PS);

		SigilPS_Comp->DestroyComponent();
	}
}

void AWeresquid::Summon_Notify()
{
	SummonFunction();
}

void AWeresquid::SummonFunction()
{
	if (Magic_Target)
	{
		int rand_distance = UKismetMathLibrary::RandomIntegerInRange(150, 850);

		FQuat rand_quat = FQuat(FRotator(0.0f, summon_yaw, 0.0f));
		FVector trace_center_loc = Magic_Target->GetActorLocation() + rand_quat.GetForwardVector() * rand_distance;

		FVector TraceStart = trace_center_loc + FVector(0.0f, 0.0f, 327.5f);
		FVector TraceEnd = trace_center_loc - FVector(0.0f, 0.0, 487.5f);
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
			FTransform SpawnTrans;
			SpawnTrans.SetLocation(HitOut.ImpactPoint + FVector(0.0f, 0.0f, 3.3f));

			FRotator tar_rot = UKismetMathLibrary::FindLookAtRotation(SpawnTrans.GetLocation(), Magic_Target->GetActorLocation());

			SpawnTrans.SetRotation(FQuat(FRotator(0.0f, tar_rot.Yaw, 0.0f)));
			SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

			ASummonCaster* SummonCaster = GetWorld()->SpawnActorDeferred<ASummonCaster>(SummonCasterClass, SpawnTrans, this, Instigator);
			SummonCaster->MyOwner = this;
			SummonCaster->AttackData = SummonAttackData;
			UGameplayStatics::FinishSpawningActor(SummonCaster, SpawnTrans);
		}

		if (summon_count < 9)
		{
			summon_yaw += 45.0f;
			summon_count += 1;

			GetWorldTimerManager().ClearTimer(SummonTimer);
			GetWorldTimerManager().SetTimer(SummonTimer, this, &AWeresquid::SummonFunction, 0.2f, false);
		}
	}
}