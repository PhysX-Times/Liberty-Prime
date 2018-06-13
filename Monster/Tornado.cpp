// Fill out your copyright notice in the Description page of Project Settings.

#include "Tornado.h"
#include "TimerManager.h"
#include "Weapon/Sword.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

ATornado::ATornado()
{
	TornadoPS_Comp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TornadoPS"));
	TornadoPS_Comp->SetupAttachment(RootComponent);
}

void ATornado::BeginPlay()
{
	Super::BeginPlay();

	AttackData = AttackData_Create(SoundCue_Attack, EImpact::Impact_Light, 1.0f);

	GetWorldTimerManager().SetTimer(ResetTornadoTimer, this, &ATornado::Reset_Tornado_Function, 0.2f, true);
}

void ATornado::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsDead)
	{
		if (TornadoTarget)
		{
			AAIController* AIConRef = Cast<AAIController>(GetController());
			AIConRef->MoveToActor(TornadoTarget);
		}

		FVector TraceStart = GetMesh()->GetComponentToWorld().GetLocation();
		FVector TraceEnd = TraceStart + FVector(0.0f, 0.0f, Trace_Height);

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

		GetWorld()->SweepMultiByChannel(HitOuts, TraceStart, TraceEnd, FQuat(), ECollisionChannel::ECC_Destructible, FCollisionShape::MakeSphere(Trace_Radius), CollisionQueryParams);

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
		}
	}
}

void ATornado::Caster_Die(AWeeper* Caster)
{
	Casters.Remove(Caster);

	if (Casters.Num() == 0)
	{
		TornadoPS_Comp->Deactivate();
		IsDead = true;
		GetWorld()->GetTimerManager().ClearTimer(ResetTornadoTimer);
		GetWorldTimerManager().SetTimer(DeathTimer, this, &ATornado::Die_Function, Death_Delay, false);
	}
}

void ATornado::Reset_Tornado_Function()
{
	DamagedChars.Empty();
}

void ATornado::Die_Function()
{
	Destroy();
}