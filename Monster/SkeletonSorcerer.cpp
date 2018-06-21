// Fill out your copyright notice in the Description page of Project Settings.

#include "SkeletonSorcerer.h"
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
#include "Miscellaneous/SummonCaster.h"
#include "Miscellaneous/IndicatorDecal.h"
#include "Miscellaneous/ArcherTrail.h"
#include "Miscellaneous/HellShell.h"
#include "DrawDebugHelpers.h"
#include "Engine.h"

ASkeletonSorcerer::ASkeletonSorcerer()
{
	can_cast = true;
	MoveTo_Distance_Add = 875.0f;
}

void ASkeletonSorcerer::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	TeleportAttackData = AttackData_Create(SoundCue_Teleport, EImpact::Impact_Heavy, 1.5f);

	RefPath = "AnimMontage'/Game/Enemy/SkeletonSorcerer/AnimationRM/SkeletonSorcerer_MagicShot_RM_Montage.SkeletonSorcerer_MagicShot_RM_Montage'";
	MagicAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/SkeletonSorcerer/AnimationRM/SkeletonSorcerer_LightSpell_RM_Montage.SkeletonSorcerer_LightSpell_RM_Montage'";
	ArrowAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Light, 0.25f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/SkeletonSorcerer/AnimationRM/SkeletonSorcerer_SpellB_RM_Montage.SkeletonSorcerer_SpellB_RM_Montage'";
	HellAttackData = AttackData_Create(RefPath, SoundCue_HellShell, ERestriction::Restriction_Full, EImpact::Impact_Light, 0.25f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/SkeletonSorcerer/AnimationRM/SkeletonSorcerer_Swing_RM_Montage.SkeletonSorcerer_Swing_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Melee, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	StaffDamager = GetWorld()->SpawnActorDeferred<ASword>(StaffDamagerClass, SpawnTrans, this, Instigator);
	StaffDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(StaffDamager, SpawnTrans);
	StaffDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("StaffSocket"));

	WeaponCluster.Add(StaffDamager);

	StaffDamager->ApplyEffect(EDamageType::Damage_Fire, true);
}

void ASkeletonSorcerer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkeletonSorcerer::Magic_Staff()
{
	Attack(MagicAttackData);
	Cast_Function(2.5f);
}

void ASkeletonSorcerer::Magic_Arrow()
{
	Attack(ArrowAttackData);
	Cast_Function(4.5f);
}

void ASkeletonSorcerer::Magic_Shell()
{
	Attack(HellAttackData);
	Cast_Function(5.5f);
}

void ASkeletonSorcerer::HellShell_Notify()
{
	FTransform SpawnTrans;
	SpawnTrans.SetLocation(Magic_Target->GetActorLocation());
	SpawnTrans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	AHellShell* HellShell = GetWorld()->SpawnActorDeferred<AHellShell>(HellShellClass, SpawnTrans, this, Instigator);
	HellShell->MyOwner = this;
	HellShell->HellAttackData = HellAttackData;
	UGameplayStatics::FinishSpawningActor(HellShell, SpawnTrans);
}

void ASkeletonSorcerer::Projectile_Notify()
{
	projectile_count = 0;
	current_yaw = UKismetMathLibrary::RandomIntegerInRange(0, 360);

	SpawnProjectile();
}

void ASkeletonSorcerer::SpawnProjectile()
{
	if (projectile_count < 4)
	{
		Play_SoundCue(SoundCue_Projectile);

		FTransform SpawnTrans;
		SpawnTrans.SetLocation(StaffDamager->GetWeapon_SK()->GetSocketLocation(FName("ProjectileSocket")));
		SpawnTrans.SetRotation(FQuat(FRotator(30.0f, current_yaw, 0.0f)));
		SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		AHomingProjectile* HomingProjectile = GetWorld()->SpawnActorDeferred<AHomingProjectile>(HomingProjectileClass, SpawnTrans, this, Instigator);
		HomingProjectile->MyOwner = this;
		HomingProjectile->DMGType = EDamageType::Damage_Fire;
		HomingProjectile->AttackData = MagicAttackData;
		HomingProjectile->HomingTarget = Magic_Target;
		UGameplayStatics::FinishSpawningActor(HomingProjectile, SpawnTrans);

		projectile_count += 1;
		current_yaw += 90;

		GetWorldTimerManager().ClearTimer(MagicTimer);
		GetWorldTimerManager().SetTimer(MagicTimer, this, &ASkeletonSorcerer::SpawnProjectile, 0.1f, false);
	}
}

void ASkeletonSorcerer::Arrow_Notify()
{
	FTransform Trans_PS;
	Trans_PS.SetLocation(StaffDamager->GetWeapon_SK()->GetSocketLocation(FName("ProjectileSocket")));
	Trans_PS.SetRotation(FQuat(FRotator(90.0, 0.0f, 0.0f)));
	Trans_PS.SetScale3D(FVector(1.75f, 1.75f, 1.75f));

	arrow_count_down = 0;
	arrow_count_up = 0;

	arrow_current_loc = Magic_Target->GetActorLocation();
	SigilPS_Comp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Sigil_PS, Trans_PS);

	Play_SoundCue_Location(SoundCue_Attack, Trans_PS.GetLocation());

	SpawnUpArrow();
	GetWorldTimerManager().ClearTimer(ArrowTimer);
	GetWorldTimerManager().SetTimer(ArrowTimer, this, &ASkeletonSorcerer::SpawnDownArrow, 1.25f, false);
}

void ASkeletonSorcerer::SpawnUpArrow()
{
	if (arrow_count_up < 111)
	{
		FTransform SpawnTrans;

		float rand_x = UKismetMathLibrary::RandomFloatInRange(-100.0f, 100.0f);
		float rand_y = UKismetMathLibrary::RandomFloatInRange(-100.0f, 100.0f);

		if (SigilPS_Comp)
		{
			SpawnTrans.SetLocation(SigilPS_Comp->GetComponentLocation() + FVector(rand_x, rand_y, 80.0f));
		}

		SpawnTrans.SetRotation(FQuat(FRotator(90.0f, 0.0f, 0.0f)));
		SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		AHomingProjectile* ArrowProjectile = GetWorld()->SpawnActorDeferred<AHomingProjectile>(ArrowProjectileClass, SpawnTrans, this, Instigator);
		ArrowProjectile->MyOwner = this;
		ArrowProjectile->DMGType = EDamageType::Damage_Fire;
		ArrowProjectile->AttackData = ArrowAttackData;
		ArrowProjectile->bArrow = true;
		ArrowProjectile->Destroy_Delay = 1.5f;
		UGameplayStatics::FinishSpawningActor(ArrowProjectile, SpawnTrans);

		if (arrow_count_up % 4 == 0)
		{
			Play_SoundCue(SoundCue_Arrow, false, FName("None"), 1.0f, ArrowProjectile->GetRootScene());
		}

		arrow_count_up += 1;

		GetWorldTimerManager().ClearTimer(ArrowUpTimer);
		GetWorldTimerManager().SetTimer(ArrowUpTimer, this, &ASkeletonSorcerer::SpawnUpArrow, 0.02f, false);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(ArrowUpTimer);
		Sigil_Destory();
	}
}

void ASkeletonSorcerer::SpawnDownArrow()
{
	if (arrow_count_down < 300)
	{
		FTransform SpawnTrans;

		float rand_x = UKismetMathLibrary::RandomFloatInRange(-100.0f, 100.0f);
		float rand_y = UKismetMathLibrary::RandomFloatInRange(-100.0f, 100.0f);

		SpawnTrans.SetLocation(arrow_current_loc + FVector(rand_x, rand_y, 1000.0f));
		SpawnTrans.SetRotation(FQuat(FRotator(-90.0f, 0.0f, 0.0f)));
		SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		AHomingProjectile* ArrowProjectile = GetWorld()->SpawnActorDeferred<AHomingProjectile>(ArrowProjectileClass, SpawnTrans, this, Instigator);
		ArrowProjectile->MyOwner = this;
		ArrowProjectile->DMGType = EDamageType::Damage_Fire;
		ArrowProjectile->AttackData = ArrowAttackData;
		ArrowProjectile->bArrow = true;
		ArrowProjectile->Destroy_Delay = 6.0f;
		UGameplayStatics::FinishSpawningActor(ArrowProjectile, SpawnTrans);

		if (arrow_count_down % 4 == 0)
		{
			Play_SoundCue(SoundCue_Arrow, false, FName("None"), 1.0f, ArrowProjectile->GetRootScene());
		}

		arrow_count_down += 1;

		if (Magic_Target)
		{
			arrow_current_loc = FVector(arrow_current_loc.X, arrow_current_loc.Y, Magic_Target->GetActorLocation().Z);
			arrow_current_loc = arrow_current_loc + FQuat(FRotator(0.0, UKismetMathLibrary::FindLookAtRotation(arrow_current_loc, Magic_Target->GetActorLocation()).Yaw, 0.0f)).GetForwardVector() * Chase_Speed;
		}

		GetWorldTimerManager().ClearTimer(ArrowDownTimer);
		GetWorldTimerManager().SetTimer(ArrowDownTimer, this, &ASkeletonSorcerer::SpawnDownArrow, 0.01f, false);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(ArrowDownTimer);
	}
}

void ASkeletonSorcerer::Sigil_Destory()
{
	if (SigilPS_Comp)
	{
		FTransform Trans_PS;

		Trans_PS.SetLocation(SigilPS_Comp->GetComponentLocation());
		Trans_PS.SetRotation(SigilPS_Comp->GetComponentQuat());
		Trans_PS.SetScale3D(FVector(1.75f, 1.75f, 1.75f));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Sigil_End_PS, Trans_PS);

		SigilPS_Comp->DestroyComponent();
	}
}

void ASkeletonSorcerer::ResetDamager_Add()
{
	GetWorld()->GetTimerManager().ClearTimer(MagicTimer);
}

void ASkeletonSorcerer::Die_Add()
{
	Super::Die_Add();

	GetWorld()->GetTimerManager().ClearTimer(ArrowTimer);
	GetWorld()->GetTimerManager().ClearTimer(ArrowUpTimer);
	GetWorld()->GetTimerManager().ClearTimer(ArrowDownTimer);
	Sigil_Destory();
}

void ASkeletonSorcerer::Magic_Teleport()
{
	PlayMontage(TeleportMontage, 1.0f, ERestriction::Restriction_Full, RotLerpSpeedNormal);
}

void ASkeletonSorcerer::Teleport_Notify()
{
	Run_EQS();
}

void ASkeletonSorcerer::Teleport_Apply()
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

		ASummonCaster* SummonCaster = GetWorld()->SpawnActorDeferred<ASummonCaster>(SummonCasterClass, Trail_Trans, this, Instigator);
		SummonCaster->MyOwner = this;
		SummonCaster->AttackData = TeleportAttackData;
		UGameplayStatics::FinishSpawningActor(SummonCaster, Trail_Trans);

		/*

		DMGType = EDamageType::Damage_Fire;
		MassDamage(200.0f, TeleportAttackData, Trail_Trans.GetLocation(), EWeaponType::Weapon_Sharp);
		DrawDebugSphere(GetWorld(), Trail_Trans.GetLocation(), 200.0f, 10, FColor::Red, false, 2.5f, 0, 2.5f);

		Trail_Trans.SetLocation(Trail_Trans.GetLocation() + FVector(0.0f, 0.0f, 5.5f));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Teleport_PS, Trail_Trans);
		*/

		Play_SoundCue_Location(SoundCue_Teleport, Trail_Trans.GetLocation());

		FRotator Magic_Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Magic_Target->GetActorLocation());
		SetActorRotation(FQuat(FRotator(0.0f, Magic_Rot.Yaw, 0.0f)));

		SetActorLocation(HitOut.ImpactPoint + FVector(0.0f, 0.0f, Cap_Height));
	}
}

void ASkeletonSorcerer::Cast_Reset()
{
	can_cast = true;
}

void ASkeletonSorcerer::Cast_Function(float cast_delay)
{
	can_cast = false;

	GetWorldTimerManager().ClearTimer(CastTimer);
	GetWorldTimerManager().SetTimer(CastTimer, this, &ASkeletonSorcerer::Cast_Reset, cast_delay, false);
}

void ASkeletonSorcerer::Run_EQS_Implementation()
{

}