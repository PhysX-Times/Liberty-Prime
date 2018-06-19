// Fill out your copyright notice in the Description page of Project Settings.

#include "Reaper.h"
#include "TimerManager.h"
#include "Weapon/Sword.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Miscellaneous/ArcherTrail.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Engine.h"

AReaper::AReaper()
{
	Earth_Rots.Init(FRotator(), 3);
	Earth_Locs.Init(FVector(), 3);
	Earth_Active.Init(false, 3);
	Earth_PSComps.Init(nullptr, 3);
	Earth_Audio_Comps.Init(nullptr, 3);
}

void AReaper::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;

	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/Reaper/Animations/REAPER_castSpellA_RM_Montage.REAPER_castSpellA_RM_Montage'";
	MagicAttackData_A = AttackData_Create(RefPath, SoundCue_GetHit, ERestriction::Restriction_Rot, EImpact::Impact_Heavy, 2.5f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/Reaper/Animations/REAPER_castSpellB_RM_Montage.REAPER_castSpellB_RM_Montage'";
	MagicAttackData_B = AttackData_Create(RefPath, SoundCue_Archer, ERestriction::Restriction_Rot, EImpact::Impact_Light, 0.25f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/Reaper/Animations/REAPER_scytheAttack1_RM_Montage.REAPER_scytheAttack1_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 0.75f, false);
	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/Reaper/Animations/REAPER_scytheAttack2_RM_Montage.REAPER_scytheAttack2_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 0.75f, false);
	RefPath = "AnimMontage'/Game/HeroicFantasyBossesVol1/Reaper/Animations/REAPER_scytheAttack3_RM_Montage.REAPER_scytheAttack3_RM_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 0.75f, false);

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetRotation(FQuat());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

	ScytheDamager = GetWorld()->SpawnActorDeferred<ASword>(ScytheDamagerClass, SpawnTrans, this, Instigator);
	ScytheDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(ScytheDamager, SpawnTrans);
	ScytheDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("ScytheSocket"));

	WeaponCluster.Add(ScytheDamager);
}

void AReaper::MagicAttack_A()
{
	Attack(MagicAttackData_A);
}

void AReaper::MagicAttack_B()
{
	Attack(MagicAttackData_B);
}

void AReaper::Earth_Notify()
{
	FRotator Init_Rot = GetActorRotation();
	earth_distance = 0.0f;

	FTransform Trans_PS;

	for (int i = 0; i < 3; i++)
	{
		Earth_Active[i] = true;
		Earth_Rots[i] = Init_Rot;
		Earth_Locs[i] = FQuat(Earth_Rots[i]).GetForwardVector() * 87.5f + GetActorLocation();
		Earth_Locs[i].Z = (GetActorLocation().Z + Magic_Target->GetActorLocation().Z) / 2.0f;
		Init_Rot += FRotator(0.0f, 120.0f, 0.0f);

		Trans_PS.SetLocation(Earth_Locs[i]);
		Trans_PS.SetRotation(FQuat(Earth_Rots[i]));
		Trans_PS.SetScale3D(FVector(1.5f, 1.5f, 1.5f));

		if (Earth_PSComps[i])
		{
			Earth_PSComps[i]->Deactivate();
		}

		Earth_Audio_Comps[i] = Play_SoundCue_Location(SoundCue_Earth_Follow, Earth_Locs[i]);
		Earth_PSComps[i] =  UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Earth_Follow_PS, Trans_PS);
	}

	Trans_PS.SetLocation(GetMesh()->GetComponentLocation());
	Trans_PS.SetRotation(FQuat(GetActorRotation()));
	Trans_PS.SetScale3D(FVector(1.25f, 1.25f, 1.25f));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Earth_Impact_PS, Trans_PS);
	Play_SoundCue_Location(SoundCue_Earth_Impact, GetActorLocation());

	GetWorldTimerManager().ClearTimer(EarthTimer);
	GetWorldTimerManager().SetTimer(EarthTimer, this, &AReaper::Earth_Function, Earth_Delay, false);
}

void AReaper::Earth_Function()
{
	if (Magic_Target)
	{
		int or_int = 0;

		for (int i = 0; i < 3; i++)
		{
			if (Earth_Active[i])
			{
				FVector TraceStart = Earth_Locs[i] + FVector(0.0f, 0.0f, 250.0f);
				FVector TraceEnd = Earth_Locs[i] - FVector(0.0f, 0.0f, 375.0f);
				FHitResult HitOut;

				TArray<AActor*> FoundActors;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALibertyPrimeCharacter::StaticClass(), FoundActors);

				FCollisionQueryParams CollisionQueryParams;
				CollisionQueryParams.bTraceAsyncScene = true;
				CollisionQueryParams.AddIgnoredActors(FoundActors);
				FCollisionResponseParams ResponseParams;
				ResponseParams.CollisionResponse.Visibility;

				GetWorld()->LineTraceSingleByChannel(HitOut, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams, ResponseParams);

				if (HitOut.bBlockingHit)
				{
					TraceEnd = HitOut.ImpactPoint + HitOut.ImpactNormal * 67.5f;

					if ((TraceEnd - Magic_Target->GetActorLocation()).Size() < 175.0f || earth_distance > 1875.0f || IsDead)
					{
						Earth_Active[i] = false;

						if (Earth_PSComps[i])
						{
							Earth_PSComps[i]->Deactivate();
						}

						if (Earth_Audio_Comps[i])
						{
							SoundComponents.Add(Earth_Audio_Comps[i]);
						}

						FTransform Trans_PS;
						Trans_PS.SetLocation(TraceEnd);
						Trans_PS.SetRotation(FQuat(Earth_Rots[i]));
						Trans_PS.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Earth_Explosion_PS, Trans_PS);

						Play_SoundCue_Location(SoundCue_Earth_Explode, TraceEnd);

						MassDamage(217.5f, MagicAttackData_A, TraceEnd, EWeaponType::Weapon_Sharp);
					}
					else
					{
						if (Earth_PSComps[i])
						{
							Earth_PSComps[i]->SetWorldLocation(TraceEnd);
							Earth_PSComps[i]->SetWorldRotation(FQuat(Earth_Rots[i]));
						}

						if (Earth_Audio_Comps[i])
						{
							Earth_Audio_Comps[i]->SetWorldLocation(TraceEnd);
						}

						Earth_Locs[i] += FQuat(Earth_Rots[i]).GetForwardVector() * Earth_Loc_Speed;
						Earth_Locs[i].Z = (GetActorLocation().Z + Magic_Target->GetActorLocation().Z) / 2.0f;

						FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(Earth_Locs[i], Magic_Target->GetActorLocation());
						FRotator NewRot = UKismetMathLibrary::RInterpTo(Earth_Rots[i], TargetRot, Earth_Delay, Earth_Rot_Speed);
						Earth_Rots[i] = FRotator(0.0f, NewRot.Yaw, 0.0f);
					}
				}
			}

			or_int = or_int + Earth_Active[i];
		}

		earth_distance += Earth_Loc_Speed;

		if (or_int > 0 || !IsDead)
		{
			GetWorldTimerManager().ClearTimer(EarthTimer);
			GetWorldTimerManager().SetTimer(EarthTimer, this, &AReaper::Earth_Function, Earth_Delay, false);
		}
	}
}

void AReaper::Archer_Notify()
{
	EndRot();
	archer_init_rot = GetActorRotation();
	archer_count = 0;

	Archers.Empty();

	GetWorldTimerManager().ClearTimer(ArcherTimer);
	ArcherFunction();
}

void AReaper::ArcherFunction()
{
	if (archer_count < 6)
	{
		archer_count += 1;
		archer_init_rot += FRotator(0.0f, 60.0f, 0.0f);

		FTransform SpawnTrans;
		SpawnTrans.SetLocation(FQuat(archer_init_rot).GetForwardVector() * 167.5f + GetActorLocation() - FVector(0.0f, 0.0f, Cap_Height));
		SpawnTrans.SetRotation(FQuat(archer_init_rot));
		SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		AArcherTrail* ArcherTrail = GetWorld()->SpawnActorDeferred<AArcherTrail>(ArcherTrailClass, SpawnTrans, this, Instigator);
		ArcherTrail->MyOwner = this;
		ArcherTrail->ArrowAttackData = MagicAttackData_B;
		UGameplayStatics::FinishSpawningActor(ArcherTrail, SpawnTrans);

		Archers.Add(ArcherTrail);

		float timer_delay = 0.1f;

		if (archer_count > 5)
		{
			timer_delay = 1.0f;
		}

		GetWorldTimerManager().ClearTimer(ArcherTimer);
		GetWorldTimerManager().SetTimer(ArcherTimer, this, &AReaper::ArcherFunction, timer_delay, false);
	}
	else
	{
		for (auto Archer : Archers)
		{
			if (Archer)
			{
				Archer->can_rot = true;
			}
		}
	}
}