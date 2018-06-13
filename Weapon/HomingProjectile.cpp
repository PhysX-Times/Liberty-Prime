// Fill out your copyright notice in the Description page of Project Settings.

#include "HomingProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"

AHomingProjectile::AHomingProjectile()
{
	bCanRot = true;
}

void AHomingProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HomingTarget)
	{
		float distance_tar = (GetActorLocation() - HomingTarget->GetActorLocation()).Size();

		if (distance_tar > Distance_Rot && bCanRot)
		{
			FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HomingTarget->GetMesh()->GetComponentLocation() + HomingTarget->Cap_Height / 2.0f);
			FRotator NewRot = UKismetMathLibrary::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, HomingSpeed_Rot);
			SetActorRotation(FQuat(NewRot));
		}
		else
		{
			bCanRot = false;
		}

		SetActorLocation(GetActorForwardVector() * HomingSpeed + GetActorLocation());
	}
}