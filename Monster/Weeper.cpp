// Fill out your copyright notice in the Description page of Project Settings.

#include "Weeper.h"
#include "Weapon/Sword.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/PhysicalMaterials/PhysicalMaterial.h"

AWeeper::AWeeper()
{

}

void AWeeper::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/Enemy/Werewolf/Animation/WEREWOLF_JumpClawAttack_Anim_RM_Montage.WEREWOLF_JumpClawAttack_Anim_RM_Montage'";
	CastAttackData = AttackData_Create(RefPath, SoundCue_GetHit, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 2.0f, 2.2f, 1.0f, false);

	int rand = UKismetMathLibrary::RandomIntegerInRange(0, 1);

	if (rand == 0)
	{
		Cast_1();
	}
	else
	{
		Cast_2();
	}
}

void AWeeper::Cast_Particle(FName SocketName, bool bTraceCheck)
{
	int rand = UKismetMathLibrary::RandomIntegerInRange(0, Cast_Particles.Num() - 1);

	FTransform Trans_PS;
	Trans_PS.SetLocation(GetMesh()->GetSocketLocation(SocketName));
	Trans_PS.SetRotation(GetMesh()->GetSocketQuaternion(SocketName));
	Trans_PS.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	if (bTraceCheck)
	{
		FVector TraceLoc = GetMesh()->GetSocketLocation(SocketName);
		FVector TraceStart = TraceLoc + FVector(0.0f, 0.0f, 50.0f);
		FVector TraceEnd = TraceLoc - FVector(0.0f, 0.0f, 87.5f);

		TArray<FHitResult> HitOuts;

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(this);
		CollisionQueryParams.bTraceAsyncScene = true;
		CollisionQueryParams.bReturnPhysicalMaterial = true;

		GetWorld()->LineTraceMultiByChannel(HitOuts, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);

		for (auto HitOut : HitOuts)
		{
			if (HitOut.bBlockingHit && HitOut.PhysMaterial->SurfaceType != EPhysicalSurface::SurfaceType8)
			{
				Trans_PS.SetLocation(HitOut.ImpactNormal * 7.5f + HitOut.ImpactPoint);
				Trans_PS.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));

				break;
			}
		}
	}

	CastParticle_Comp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Cast_Particles[rand], Trans_PS, true);
	CastParticle_Comp->SetColorParameter(FName("SigilColor"), CastPS_Color);
	ParticleCluster.Add(CastParticle_Comp);
}

void AWeeper::Cast_1()
{
	GetMesh()->GetAnimInstance()->Montage_Play(Cast1_Montage);
}

void AWeeper::Cast_2()
{
	GetMesh()->GetAnimInstance()->Montage_Play(Cast2_Montage);
}

void AWeeper::CastAttack()
{
	Attack(CastAttackData);
}

void AWeeper::Die_Add()
{
	Super::Die_Add();


}