// Fill out your copyright notice in the Description page of Project Settings.

#include "MasterGrunt.h"
#include "Weapon/Sword.h"
#include "TimerManager.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Projectile.h"

AMasterGrunt::AMasterGrunt()
{

}

void AMasterGrunt::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Master_Grunt/Enemy_Master_Grunt_Animation/ExoGame_MasterGrunt_Attack_Melee_B_Montage.ExoGame_MasterGrunt_Attack_Melee_B_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 3.75f, 1.0f, false);
	RefPath = "AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Master_Grunt/Enemy_Master_Grunt_Animation/ExoGame_MasterGrunt_Attack_Melee_C_Montage.ExoGame_MasterGrunt_Attack_Melee_C_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 3.75f, 1.0f, false);
	RefPath = "AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Master_Grunt/Enemy_Master_Grunt_Animation/ExoGame_MasterGrunt_Attack_Melee_Montage.ExoGame_MasterGrunt_Attack_Melee_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 3.75f, 1.0f, false);

	RefPath = "AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Master_Grunt/Enemy_Master_Grunt_Animation/ExoGame_MasterGrunt_Attack_Ranged_Montage.ExoGame_MasterGrunt_Attack_Ranged_Montage'";
	CannonAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Heavy, 3.5f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Master_Grunt/Enemy_Master_Grunt_Animation/ExoGame_MasterGrunt_Cast_Heal_Montage.ExoGame_MasterGrunt_Cast_Heal_Montage'";
	HealAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Light, 1.0f, 2.2f, 1.0f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	RightHandDamager = GetWorld()->SpawnActorDeferred<ASword>(HandDamagerClass, SpawnTrans, this, Instigator);
	RightHandDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(RightHandDamager, SpawnTrans);
	RightHandDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightHandSocket"));

	LeftHandDamager = GetWorld()->SpawnActorDeferred<ASword>(HandDamagerClass, SpawnTrans, this, Instigator);
	LeftHandDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(LeftHandDamager, SpawnTrans);
	LeftHandDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftHandSocket"));

	WeaponCluster.Add(RightHandDamager);
	WeaponCluster.Add(LeftHandDamager);

}

void AMasterGrunt::CannonAttack()
{
	Attack(CannonAttackData);
}

void AMasterGrunt::Cannon_Fire()
{
	if (Cannon_Target)
	{
		float Target_Pitch = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Cannon_Target->GetActorLocation() - FVector(0.0f, 0.0f, Cannon_Target->Cap_Height)).Pitch;

		FTransform SpawnTrans;
		SpawnTrans.SetLocation(GetMesh()->GetSocketLocation("CannonSocket"));
		SpawnTrans.SetRotation(FQuat(FRotator(Target_Pitch, GetActorRotation().Yaw, 0.0f)));
		SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		AProjectile* LocalProjectile = GetWorld()->SpawnActorDeferred<AProjectile>(ProjectileClass, SpawnTrans, this, Instigator);
		LocalProjectile->MyOwner = this;
		LocalProjectile->weapon_type = EWeaponType::Weapon_None;
		LocalProjectile->AttackData = CannonAttackData;
		UGameplayStatics::FinishSpawningActor(LocalProjectile, SpawnTrans);

		SpawnTrans.SetRotation(FQuat(GetMesh()->GetSocketRotation("CannonSocket")));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CannonMuzzle_PS, SpawnTrans);

		Play_SoundCue(SoundCue_Cannon, false);
	}
}

void AMasterGrunt::Impact_Ground(FVector TraceLoc, bool bMassDamage)
{
	FVector TraceStart = TraceLoc + FVector(0.0f, 0.0f, 67.5f);
	FVector TraceEnd = TraceLoc - FVector(0.0f, 0.0f, 75.0f);

	FHitResult HitOut;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	CollisionQueryParams.bTraceAsyncScene = true;

	GetWorld()->LineTraceSingleByChannel(HitOut, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);

	if (HitOut.bBlockingHit)
	{
		FTransform Trans_PS;
		Trans_PS.SetLocation(HitOut.ImpactNormal * 7.5f + HitOut.ImpactPoint);
		Trans_PS.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
		Trans_PS.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Impact_PS, Trans_PS);

		if (bMassDamage)
		{
			Play_SoundCue(SoundCue_Impact, false);
		}
	}
}

void AMasterGrunt::Cast_Heal()
{
	Attack(HealAttackData);
}

void AMasterGrunt::Heal_Notify()
{
	FTransform Trans_PS;
	Trans_PS.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
	Trans_PS.SetScale3D(FVector(2.5f, 2.5f, 2.5f));
	Trans_PS.SetLocation(GetMesh()->GetComponentLocation());

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Heal_PS, Trans_PS);

	Trans_PS.SetLocation(RightHandDamager->GetElementalPSComp()->GetComponentToWorld().GetLocation());
	Trans_PS.SetScale3D(FVector(1.75f, 1.75f, 1.75f));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AxeMuzzle_PS, Trans_PS);


	float Health_Add_Val = MaxHealth / 5.0f;
	Health = UKismetMathLibrary::FClamp(Health + Health_Add_Val, 0.0f, MaxHealth);

	if (PlayerController)
	{
		PlayerController->Add_Indicator(Health_Add_Val, HealColor, GetActorLocation());
	}

	Play_SoundCue(SoundCue_Axe, false);
}

void AMasterGrunt::Impact_Start()
{
	Play_SoundCue(SoundCue_Ground, true);
	GetWorldTimerManager().SetTimer(ImpactTimer, this, &AMasterGrunt::ImpactFunction, 0.1f, true);
}

void AMasterGrunt::Impact_End()
{
	SoundActive_Destroy();
	GetWorld()->GetTimerManager().ClearTimer(ImpactTimer);
}

void AMasterGrunt::ImpactFunction()
{
	Impact_Ground(RightHandDamager->GetElementalPSComp()->GetComponentToWorld().GetLocation(), false);
	Impact_Ground(LeftHandDamager->GetElementalPSComp()->GetComponentToWorld().GetLocation(), false);
}

void AMasterGrunt::Impact_Right_Sound()
{
	Impact_Ground(RightHandDamager->GetElementalPSComp()->GetComponentToWorld().GetLocation(), true);
}

void AMasterGrunt::Impact_Left_Sound()
{
	Impact_Ground(LeftHandDamager->GetElementalPSComp()->GetComponentToWorld().GetLocation(), true);
}

void AMasterGrunt::ResetDamager_Add()
{
	GetWorld()->GetTimerManager().ClearTimer(ImpactTimer);
}