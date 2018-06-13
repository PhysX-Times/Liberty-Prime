// Fill out your copyright notice in the Description page of Project Settings.

#include "Chimera.h"
#include "Weapon/Sword.h"
#include "TimerManager.h"
#include "Miscellaneous/AttackData.h"
#include "Miscellaneous/IndicatorDecal.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Controller/MainAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

AChimera::AChimera()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/Enemy/Chimera/Animation/CHIMERA_Death_Anim_Montage.CHIMERA_Death_Anim_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj(TEXT("AnimMontage'/Game/Enemy/Chimera/Animation/CHIMERA_Hit_Anim_Montage.CHIMERA_Hit_Anim_Montage'"));
	HitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));
	HeavyHitDatas.Add(FMontageData(HitMontageObj.Object, 0.75f));
}

void AChimera::BeginPlay()
{
	Super::BeginPlay();

	FString RefPath;
	RefPath = "AnimMontage'/Game/Enemy/Chimera/Animation/CHIMERA_Claw2HitCombo_Montage.CHIMERA_Claw2HitCombo_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/Chimera/Animation/CHIMERA_ClawAttackL_Anim_Montage.CHIMERA_ClawAttackL_Anim_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/Chimera/Animation/CHIMERA_SnakeBite_Montage.CHIMERA_SnakeBite_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.35f, 2.2f, 1.0f, false);

	RefPath = "AnimMontage'/Game/Enemy/Chimera/Animation/CHIMERA_RamBite_Anim_Montage.CHIMERA_RamBite_Anim_Montage'";
	RamAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.5f, 2.2f, 1.0f, false);

	CastData = AttackData_Create(SoundCue_Attack, EImpact::Impact_Normal, 0.0f);

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

	SnakeDamager = GetWorld()->SpawnActorDeferred<ASword>(SnakeDamagerClass, SpawnTrans, this, Instigator);
	SnakeDamager->MyOwner = this;
	SnakeDamager->DMGType = EDamageType::Damage_Poison;
	UGameplayStatics::FinishSpawningActor(SnakeDamager, SpawnTrans);
	SnakeDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("SnakeSocket"));

	BiteDamager = GetWorld()->SpawnActorDeferred<ASword>(BiteDamagerClass, SpawnTrans, this, Instigator);
	BiteDamager->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(BiteDamager, SpawnTrans);
	BiteDamager->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("BiteSocket"));

	GoatPSComp = UGameplayStatics::SpawnEmitterAttached(GoatPS, GetMesh(), FName("GoatSocket"));

	ParticleCluster.Add(GoatPSComp);

	WeaponCluster.Add(SnakeDamager);
	WeaponCluster.Add(BiteDamager);
	WeaponCluster.Add(LeftHandDamager);
	WeaponCluster.Add(RightHandDamager);

	ChimeraCast();
}

void AChimera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChimera::RamAttack()
{
	Attack(RamAttackData);
}

void AChimera::ChimeraCast()
{
	AMainAIController* AIConRef = Cast<AMainAIController>(GetController());

	if (!IsDead)
	{
		GetWorldTimerManager().SetTimer(CastTimer, this, &AChimera::ChimeraCast, 2.0f, false);

		if (AIConRef)
		{
			ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(AIConRef->GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(AIConRef->EnemyKeyID));

			if (IsTarget)
			{
				FVector TraceStart = IsTarget->GetActorLocation();
				FVector TraceEnd = TraceStart - FVector(0.0f, 0.0f, 500.0f);
				FHitResult HitOut;

				FCollisionQueryParams CollisionQueryParams;
				CollisionQueryParams.AddIgnoredActor(IsTarget);
				CollisionQueryParams.bTraceAsyncScene = true;
				FCollisionResponseParams ResponseParams;
				ResponseParams.CollisionResponse.Visibility;

				GetWorld()->LineTraceSingleByChannel(HitOut, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams, ResponseParams);

				if (HitOut.bBlockingHit)
				{
					IndicatorTrans.SetLocation(HitOut.ImpactPoint + HitOut.ImpactNormal * 12.5f);
					IndicatorTrans.SetRotation(FQuat(HitOut.ImpactNormal.Rotation()));
					IndicatorTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

					AIndicatorDecal* MyIndicator = GetWorld()->SpawnActorDeferred<AIndicatorDecal>(IndicatorClass, IndicatorTrans, this, Instigator);
					UGameplayStatics::FinishSpawningActor(MyIndicator, IndicatorTrans);

					GetWorldTimerManager().SetTimer(SummonTimer, this, &AChimera::Summon, 1.5f, false);
				}
			}
		}
	}
}

void AChimera::Summon()
{
	IndicatorTrans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CastPS, IndicatorTrans, true);
	Play_SoundCue_Location(SoundCue_Summon, IndicatorTrans.GetLocation());
	MassDamage(180.0f, CastData, IndicatorTrans.GetLocation(), EWeaponType::Weapon_Sharp);
}