// Fill out your copyright notice in the Description page of Project Settings.

#include "PlantMonster.h"
#include "TimerManager.h"
#include "Weapon/Sword.h"
#include "Weapon/HomingProjectile.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"

APlantMonster::APlantMonster()
{

}

void APlantMonster::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	int rand = UKismetMathLibrary::RandomIntegerInRange(0, PlantMonster_Mats.Num() - 1);

	GetMesh()->SetMaterial(0, PlantMonster_Mats[rand].MainMat);
	GetMesh()->SetMaterial(1, PlantMonster_Mats[rand].VinesMat);
	GetMesh()->SetMaterial(2, PlantMonster_Mats[rand].FlowersMat);
	GetMesh()->SetMaterial(3, PlantMonster_Mats[rand].ThornsMat);

	RefPath = "AnimMontage'/Game/Enemy/PlantMonster/Animation/PlantMonster_MagicCharge_Montage.PlantMonster_MagicCharge_Montage'";
	MagicAttackData = AttackData_Create(RefPath, SoundCue_Charge, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.1f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/PlantMonster/Animation/PlantMonster_Attack1_Montage.PlantMonster_Attack1_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/Enemy/PlantMonster/Animation/PlantMonster_Attack2_Montage.PlantMonster_Attack2_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack1, ERestriction::Restriction_Full, EImpact::Impact_Heavy, 2.0f, 2.2f, 1.0f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	JawDamager = GetWorld()->SpawnActorDeferred<ASword>(JawDamagerClass, SpawnTrans, this, Instigator);
	JawDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(JawDamager, SpawnTrans);
	JawDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("JawSocket"));

	WeaponCluster.Add(JawDamager);
}

void APlantMonster::MagicAttack()
{
	Attack(MagicAttackData);
}

void APlantMonster::Spawn_HomingProjectiles()
{
	projectile_count = 0;
	current_yaw = 0;

	Spawn_Projectile_Function();
	Play_SoundCue(SoundCue_Projectile);

	FTransform Trans_PS;
	Trans_PS.SetLocation(GetMesh()->GetSocketLocation("ProjectileSocket"));
	Trans_PS.SetRotation(FQuat());
	Trans_PS.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Projectile_PS, Trans_PS);
}

void APlantMonster::Spawn_Projectile_Function()
{
	if (projectile_count < 4)
	{
		FTransform SpawnTrans;

		SpawnTrans.SetLocation(GetMesh()->GetSocketLocation(FName("ProjectileSocket")));
		SpawnTrans.SetRotation(FQuat(FRotator(60.0f, current_yaw, 0.0f)));
		SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		AHomingProjectile* HomingProjectile = GetWorld()->SpawnActorDeferred<AHomingProjectile>(HomingProjectileClass, SpawnTrans, this, Instigator);
		HomingProjectile->MyOwner = this;
		HomingProjectile->DMGType = EDamageType::Damage_Poison;
		HomingProjectile->AttackData = MagicAttackData;
		HomingProjectile->HomingTarget = HomingTarget;
		UGameplayStatics::FinishSpawningActor(HomingProjectile, SpawnTrans);

		projectile_count += 1;
		current_yaw += 90;

		GetWorldTimerManager().SetTimer(MagicTimer, this, &APlantMonster::Spawn_Projectile_Function, 0.15f, false);
	}
}