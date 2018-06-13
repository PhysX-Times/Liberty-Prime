// Fill out your copyright notice in the Description page of Project Settings.

#include "RockMonster.h"
#include "Weapon/Sword.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "DrawDebugHelpers.h"
#include "Engine.h"

ARockMonster::ARockMonster()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/Enemy/RockMonster/Animation/Rock_Monster_Death_Montage.Rock_Monster_Death_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj(TEXT("AnimMontage'/Game/Enemy/RockMonster/Animation/Rock_Monster_GetHit_Montage.Rock_Monster_GetHit_Montage'"));
	HeavyHitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BearMat1(TEXT("MaterialInstanceConstant'/Game/Enemy/RockMonster/Material/RockMonster_Custom1_Inst.RockMonster_Custom1_Inst'"));
	RockMats.Add(BearMat1.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BearMat2(TEXT("MaterialInstanceConstant'/Game/Enemy/RockMonster/Material/RockMonster_Custom2_Inst.RockMonster_Custom2_Inst'"));
	RockMats.Add(BearMat2.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BearMat3(TEXT("MaterialInstanceConstant'/Game/Enemy/RockMonster/Material/RockMonster_Rock1_Inst.RockMonster_Rock1_Inst'"));
	RockMats.Add(BearMat3.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BearMat4(TEXT("MaterialInstanceConstant'/Game/Enemy/RockMonster/Material/RockMonster_Rock2_Inst.RockMonster_Rock2_Inst'"));
	RockMats.Add(BearMat4.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BearMat5(TEXT("MaterialInstanceConstant'/Game/Enemy/RockMonster/Material/RockMonster_Rock3_Inst.RockMonster_Rock3_Inst'"));
	RockMats.Add(BearMat5.Object);

	bBuildUp = true;
}

void ARockMonster::BeginPlay()
{
	Super::BeginPlay();

	IsInvincible = true;

	int rand = UKismetMathLibrary::RandomIntegerInRange(0, RockMats.Num() - 1);
	GetMesh()->SetMaterial(0, RockMats[rand]);

	FString RefPath;

	RefPath = "AnimMontage'/Game/Enemy/RockMonster/Animation/Rock_Monster_Magic_Montage.Rock_Monster_Magic_Montage'";
	LaserAttackData = AttackData_Create(RefPath, SoundCue_Attack_Laser, ERestriction::Restriction_Rot, EImpact::Impact_Heavy, 0.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/RockMonster/Animation/Rock_Monster_Attack1_Montage.Rock_Monster_Attack1_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 3.3f, 1.0f, false);
	RefPath = "AnimMontage'/Game/Enemy/RockMonster/Animation/Rock_Monster_Attack2_Montage.Rock_Monster_Attack2_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 3.3f, 1.0f, false);
	RefPath = "AnimMontage'/Game/Enemy/RockMonster/Animation/Rock_Monster_Attack3_Montage.Rock_Monster_Attack3_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack1, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 3.75f, 1.0f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	RightHandDamager = GetWorld()->SpawnActorDeferred<ASword>(HandDamagerClass, SpawnTrans, this, Instigator);
	RightHandDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(RightHandDamager, SpawnTrans);
	RightHandDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightClawSocket"));

	LeftHandDamager = GetWorld()->SpawnActorDeferred<ASword>(HandDamagerClass, SpawnTrans, this, Instigator);
	LeftHandDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(LeftHandDamager, SpawnTrans);
	LeftHandDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftClawSocket"));

	WeaponCluster.Add(RightHandDamager);
	WeaponCluster.Add(LeftHandDamager);

	GetMesh()->GetAnimInstance()->Montage_Play(Rubble_Idle_Montage);

	LightningDMG *= 2.0f;

}

void ARockMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bLaserActive)
	{
		FQuat SocketRot;
		FVector SocketLoc;
		GetMesh()->GetSocketWorldLocationAndRotation(FName("LaserSocket"), SocketLoc, SocketRot);

		SocketRot.GetForwardVector();
		FVector TraceEnd = SocketLoc + SocketRot.GetForwardVector() * 1750.0f;
		TArray<FHitResult> HitOuts;
		FCollisionResponseParams ResponseParams;
		ResponseParams.CollisionResponse.Visibility;

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

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActors(FoundActors);
		CollisionQueryParams.bTraceAsyncScene = true;

		GetWorld()->LineTraceMultiByChannel(HitOuts, SocketLoc, TraceEnd, ECollisionChannel::ECC_Destructible, CollisionQueryParams, ResponseParams);
		//DrawDebugLine(GetWorld(), SocketLoc, TraceEnd, FColor(255, 0, 0), false, -1, 0, 10);

		for (auto HitOut : HitOuts)
		{
			ALibertyPrimeCharacter* IsLPC = Cast<ALibertyPrimeCharacter>(HitOut.GetActor());

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
					IsLPC->Damager(BakeDamageData(this, AttackData, DMGType, EWeaponType::Weapon_None), IsLPC->GetMesh()->GetBoneLocation(HitOut.BoneName));
					DamagedChars.AddUnique(IsLPC);
				}
			}
			else
			{
				if (HitOut.bBlockingHit)
				{
					TraceEnd = HitOut.Location + HitOut.ImpactNormal * 17.5f;
				}

				LaserPS_Comp->SetBeamSourcePoint(0, SocketLoc, 0);
				LaserPS_Comp->SetBeamTargetPoint(0, TraceEnd, 0);
				LaserPS_End_Comp->SetWorldLocation(TraceEnd);
				break;
			}
		}
	}
}

void ARockMonster::LaserAttack()
{
	Attack(LaserAttackData);
}

void ARockMonster::Laser_Start()
{
	FTransform LaserTrans;
	LaserTrans.SetLocation(GetMesh()->GetSocketLocation(FName("LaserSocket")));
	LaserTrans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
	LaserTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
	bLaserActive = true;

	LaserPS_Comp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaserParticle, LaserTrans, true);
	LaserTrans.SetScale3D(FVector(0.8f, 0.8f, 0.8f));
	LaserPS_End_Comp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaserParticle_End, LaserTrans, true);

	ParticleCluster.Add(LaserPS_Comp);
	ParticleCluster.Add(LaserPS_End_Comp);
}

void ARockMonster::Laser_End()
{
	DamagedChars.Empty();
	bLaserActive = false;
	LaserPS_Comp->Deactivate();
	LaserPS_End_Comp->Deactivate();
	ParticleCluster.Remove(LaserPS_Comp);
	ParticleCluster.Remove(LaserPS_End_Comp);
}

void ARockMonster::Build_Up()
{
	PlayMontage(Pose_Montage, 1.0f, ERestriction::Restriction_Full, RotLerpSpeedNormal);
	Play_SoundCue_Location(SoundCue_GetPose, GetActorLocation());
}

void ARockMonster::Pose_End()
{
	IsInvincible = false;
	Pose_Notify();
	ResetMontage();
}

void ARockMonster::ResetDamager_Add()
{
	bLaserActive = false;

	if (LaserPS_Comp)
	{
		LaserPS_Comp->Deactivate();
	}

	if (LaserPS_End_Comp)
	{
		LaserPS_End_Comp->Deactivate();
	}

	ParticleCluster.Remove(LaserPS_Comp);
	ParticleCluster.Remove(LaserPS_End_Comp);
}

void ARockMonster::Pose_Notify_Implementation()
{

}