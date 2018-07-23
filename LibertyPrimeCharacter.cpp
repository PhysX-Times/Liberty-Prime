// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#include "LibertyPrimeCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Controller/MainAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Miscellaneous/BloodDecal.h"
#include "Miscellaneous/AttackData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "UObject/ConstructorHelpers.h"
#include "Weapon/Sword.h"
#include "Weapon/Weapon.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "Runtime/Landscape/Classes/Landscape.h"
#include "Player/PlayerChar.h"
#include "Miscellaneous/Item.h"
#include "LibertyPrimePlayerController.h"
#include "Engine.h"

ALibertyPrimeCharacter::ALibertyPrimeCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	IsDead = false;
	bShield = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	PrimaryActorTick.bCanEverTick = true;

	RotLerpSpeed = 5.5f;
	RotLerpSpeedNormal = 5.5f;
	restriction = ERestriction::Restriction_None;
	DMGType = EDamageType::Damage_None;
	ElementalInterval = 0.5f;
	PhysicalResist_DMG = 5.0f;
	PoisonResist_DMG = 0.1f;
	FireResist_DMG = 2.0f;
	FreezeResist_Frostbite = 0.05f;
	FreezeDuration = 5.0f;
	LightningResist_DMG = 4.0f;
	Lightning_Percentage = 15.0f;

	FireTick = 4;
	PoisonTick = 60;

	SpeedUpAmount = 0.05f;
	FreezeAmount = 0.25f;
	SpeedUpMax = 0.35f;

	MoveTo_Distance_Add = 7.5f;

	BloodSize_Decal = 1.0f;
	BloodSize_PS = FVector(1.0f, 1.0f, 1.0f);
	BloodColor = FLinearColor(0.300000, 0.018402, 0.012432, 1.000000);

	PhysicalColor = FLinearColor(1.000000, 1.000000, 1.000000, 1.000000);
	FireColor = FLinearColor(1.000000, 0.167170, 0.000000, 1.000000);
	PoisonColor = FLinearColor(0.411905, 0.7250000, 0.113235, 1.000000);
	LightningColor = FLinearColor(0.000000, 0.538882, 0.875000, 1.000000);
	IceColor = FLinearColor(1.000000, 0.131321, 0.000000, 1.000000);
	HealColor = FLinearColor(1.000000, 0.000000, 0.000000, 1.000000);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	DeathLine_RefPath = "CurveFloat'/Game/AI/DeathCurve.DeathCurve'";
	FootstepVolume = 1.0f;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->bGenerateOverlapEvents = false;

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 210.0f, 0.0f);

	AgroCheckRadius = 1500.0f;
}

void ALibertyPrimeCharacter::BeginPlay()
{
	Super::BeginPlay();

	Cap_Height = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	RandomAttacks.Empty();

	BodyParts_SK.Empty();
	BodyParts_SK.Add(GetMesh());

	FStringAssetReference CurveFloatRef("CurveFloat'/Game/AI/ElementalCurve.ElementalCurve'");
	ElementalCurveFloat = Cast<UCurveFloat>(CurveFloatRef.TryLoad());

	InitializeFunction(&IceLine, FName("IceFunction"), ElementalCurveFloat);
	InitializeFunction(&FireLine, FName("FireFunction"), ElementalCurveFloat);
	InitializeFunction(&PoisonLine, FName("PoisonFunction"), ElementalCurveFloat);
	InitializeFunction(&LightningLine, FName("LightningFunction"), ElementalCurveFloat);

	FStringAssetReference FresnelCurveFloatRef("CurveFloat'/Game/AI/FresnelCurve.FresnelCurve'");
	FresnelCurveFloat = Cast<UCurveFloat>(FresnelCurveFloatRef.TryLoad());
	InitializeFunction(&FresnelLine, FName("FresnelFunction"), FresnelCurveFloat);

	FStringAssetReference DeathCurveFloatRef(DeathLine_RefPath);
	DeathCurveFloat = Cast<UCurveFloat>(DeathCurveFloatRef.TryLoad());
	InitializeFunction(&DeathLine, FName("DeathFunction"), DeathCurveFloat);

	if (DoesBreathe)
	{
		Breathe();
	}

	PlayerController = Cast<ALibertyPrimePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void ALibertyPrimeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotLerp(DeltaTime);

	TL_CheckTick(&IceLine, DeltaTime);
	TL_CheckTick(&FireLine, DeltaTime);
	TL_CheckTick(&LightningLine, DeltaTime);
	TL_CheckTick(&PoisonLine, DeltaTime);
	TL_CheckTick(&FresnelLine, DeltaTime);
	TL_CheckTick(&DeathLine, DeltaTime);

	for (auto SoundComponent : SoundComponents)
	{
		if (SoundComponent)
		{
			float CompVol = SoundComponent->VolumeMultiplier;
			CompVol = FMath::Clamp(CompVol - 0.015f, 0.0f, 10.0f);
			SoundComponent->SetVolumeMultiplier(CompVol);

			if (CompVol <= 0.0f)
			{
				SoundComponent->DestroyComponent();
			}
		}
		else
		{
			SoundComponents.Remove(SoundComponent);
		}
	}
}

void ALibertyPrimeCharacter::Attack(UAttackData* TargetData)
{
	if (TargetData)
	{
		AttackData = TargetData;

		if (TargetData->IsSequencer)
		{
			PlayMontage(TargetData->AttackMontages[AttackSequenceCount], TargetData->PlaySpeed, TargetData->restriction, TargetData->RotSpeed);
		}
		else
		{
			PlayMontage(TargetData->AttackMontage, TargetData->PlaySpeed, TargetData->restriction, TargetData->RotSpeed);
		}

		AttackSequenceCount += 1;
	}
}

void ALibertyPrimeCharacter::AttackCheck()
{
	AMainAIController* IsCon = Cast<AMainAIController>(GetController());
	
	if (IsCon)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(IsCon->GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(IsCon->EnemyKeyID));

		if (IsTarget)
		{
			float Distance = (GetActorLocation() - IsTarget->GetActorLocation()).Size() - GetCapsuleComponent()->GetScaledCapsuleRadius() - IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();

			if (Distance <= AttackDistance && IsCon->NormalAttackCurrent < IsCon->NormalAttackMax)
			{
				IsCon->NormalAttackCurrent += 1;
			}
			else
			{
				GetMesh()->GetAnimInstance()->Montage_Stop(0.25f);
				ResetMontage();
				AttackSequenceCount = 0;
				ResetDamager();
			}
		}
	}
}

void ALibertyPrimeCharacter::AttackSequenceCheck()
{
	AMainAIController* IsCon = Cast<AMainAIController>(GetController());

	if (IsCon)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(IsCon->GetBlackboardComp()->GetValue<UBlackboardKeyType_Object>(IsCon->EnemyKeyID));

		if (IsTarget)
		{
			float Distance = (GetActorLocation() - IsTarget->GetActorLocation()).Size() - GetCapsuleComponent()->GetScaledCapsuleRadius() - IsTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();

			if (Distance <= AttackDistance && IsCon->NormalAttackCurrent < IsCon->NormalAttackMax)
			{
				Attack(AttackData);
				IsCon->NormalAttackCurrent += 1;
			}
			else
			{
				AttackSequenceCount = 0;
				ResetMontage();
			}
		}
	}
}

void ALibertyPrimeCharacter::ResetSequenceCount()
{
	AttackSequenceCount = 0;
}

void ALibertyPrimeCharacter::Damager(FDamageData DamageData, FVector PSLoc)
{
	if (!IsDead)
	{
		float dmg_multiplied = DamageData.Applier->DMG * DamageData.AttackData->DMGMultiplier;
		float TargetDMG = Calculate_Effect(dmg_multiplied, PhysicalResist_DMG);

		APlayerChar* IsPlayerChar = Cast<APlayerChar>(DamageData.Applier);

		if (TargetDMG != 0.0f && PlayerController)
		{
			PlayerController->Add_Indicator(TargetDMG, PhysicalColor, PSLoc);
		}

		if (DamageData.DMGType == EDamageType::Damage_Fire)
		{
			float FireDMG_Local = Calculate_Effect(DamageData.Applier->FireDMG / 2.0f, FireResist_DMG / 2.0f);
			TargetDMG += FireDMG_Local;

			if (PlayerController)
			{
				PlayerController->Add_Indicator(FireDMG_Local, FireColor, PSLoc + FVector(0.0f, 0.0f, 47.5f));
			}
		}
		else if (DamageData.DMGType == EDamageType::Damage_Lightning)
		{
			float LightningDMG_Local = Calculate_Effect(DamageData.Applier->LightningDMG, LightningResist_DMG);

			DamageData.Applier->Damager_Simple(LightningDMG_Local / 100.0f * DamageData.Applier->Lightning_Percentage, LightningColor);
			TargetDMG += LightningDMG_Local;

			if (PlayerController)
			{
				PlayerController->Add_Indicator(LightningDMG_Local, LightningColor, PSLoc + FVector(0.0f, 0.0f, 47.5f));
			}
		}

		if (IsPlayerChar && IsPlayerChar->Lifesteal_Rate > 0.0f)
		{
			IsPlayerChar->Lifesteal(TargetDMG / 100.0f * IsPlayerChar->Lifesteal_Rate);
		}

		if (bShield)
		{
			FVector Offset = (GetActorLocation() - DamageData.Source->GetActorLocation()).GetSafeNormal();

			float DotProduct = FVector::DotProduct(GetActorForwardVector(), Offset);

			if (DotProduct >= 0.0f)
			{
				TargetDMG = TargetDMG * 0.7f;
			}
		}

		Health = UKismetMathLibrary::FClamp(Health - TargetDMG, 0.0f, MaxHealth);
		UpdateHealth_PB();

		if (Health <= 0.0f)
		{
			IsDead = true;
		}

		if (DamageData.AttackData->impact != EImpact::Impact_Light)
		{
			SpawnBloodDecal(DamageData.Source->GetActorLocation(), PSLoc);
		}

		FTransform HitPSTrans;
		HitPSTrans.SetLocation(PSLoc);
		HitPSTrans.SetScale3D(BloodSize_PS);
		HitPSTrans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitPS, HitPSTrans, true);

		switch (DamageData.WeaponType)
		{
		case EWeaponType::Weapon_Blunt:
			Play_SoundCue(SoundCue_Hit_Blunt, false);
			break;
		case EWeaponType::Weapon_Sharp:
			Play_SoundCue(SoundCue_Hit_Sharp, false);
			break;
		case EWeaponType::Weapon_Claw:
			Play_SoundCue(SoundCue_Hit_Claw, false);
			break;
		default:
			break;
		}

		if (DamageData.AttackData->impact == EImpact::Impact_Heavy)
		{
			if (!IsDead && CanBeHeavyHit)
			{
				ResetDamager();
				int rand = UKismetMathLibrary::RandomIntegerInRange(0, HeavyHitDatas.Num() - 1);
				PlayMontage(HeavyHitDatas[rand].TargetMontage, HeavyHitDatas[rand].PlaySpeed, ERestriction::Restriction_Full, RotLerpSpeed);

				if (CanHeavyHitRot)
				{
					FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DamageData.Source->GetActorLocation());
					SetActorRotation(FQuat(FRotator(0.0f, NewRot.Yaw, 0.0f)));
				}

				if (bHeavyRigidBody)
				{
					IsRigidBody = true;
				}

				if (bHeavyInvincible)
				{
					IsInvincible = true;
				}

				SoundActive_Destroy();
				Play_SoundCue(SoundCue_GetHit, true);
			}
		}
		else if (DamageData.AttackData->impact == EImpact::Impact_Normal)
		{
			if (!IsDead && CanBeNormalHit)
			{
				ResetDamager();
				int rand = FMath::RandRange(0, HitDatas.Num() - 1);
				PlayMontage(HitDatas[rand].TargetMontage, HitDatas[rand].PlaySpeed, ERestriction::Restriction_Full, RotLerpSpeed);

				SoundActive_Destroy();
				Play_SoundCue(SoundCue_GetHit, true);
			}
		}

		if (push_cond != EICondition::Condition_None)
		{
			if (push_cond == EICondition::Condition_Both || int(push_cond) == int(DamageData.AttackData->impact))
			{
				FVector SourceLoc = DamageData.Source->GetActorLocation();
				FVector PushNormal = (GetActorLocation() - FVector(SourceLoc.X, SourceLoc.Y, GetActorLocation().Z)).GetSafeNormal();
				FVector TraceStart = GetActorLocation();
				FVector TraceEnd = TraceStart + (PushNormal * Weight);
				FHitResult HitOut;
				FCollisionQueryParams QueryParams;
				QueryParams.AddIgnoredActor(this);

				GetWorld()->LineTraceSingleByChannel(HitOut, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, QueryParams);

				if (HitOut.bBlockingHit)
				{
					TraceEnd = HitOut.ImpactPoint + HitOut.ImpactNormal * 77.5f;
				}

				FLatentActionInfo LatentInfo;
				LatentInfo.CallbackTarget = this;
				UKismetSystemLibrary::MoveComponentTo(RootComponent, TraceEnd, GetActorRotation(), false, false, 0.22f, false, EMoveComponentAction::Type::Move, LatentInfo);
			}
		}

		if (IsDead)
		{
			Die();
		}

		ApplyElemental(DamageData.Applier, DamageData.DMGType);
	}
}

void ALibertyPrimeCharacter::Damager_Simple(float DMG, FLinearColor Color)
{
	Health = UKismetMathLibrary::FClamp(Health - DMG, 0.0f, MaxHealth);

	if (PlayerController)
	{
		PlayerController->Add_Indicator(DMG, Color, GetActorLocation());
	}
	
	UpdateHealth_PB();

	if (Health <= 0.0f)
	{
		IsDead = true;
	}

	if (IsDead)
	{
		Die();
	}
}

void ALibertyPrimeCharacter::SpawnBloodDecal(FVector SourceLoc, FVector ImpactLoc)
{
	if (bSpawnBlood)
	{
		FRotator BloodRot = FRotator(0.0f, 0.0f, 0.0f);
		FVector TraceNormal = (ImpactLoc - SourceLoc).GetSafeNormal();
		FVector TraceOffset = TraceNormal * 17.5f + ImpactLoc + FVector(0.0f, 0.0f, 200.0f);
		FVector TraceStart = FVector(TraceOffset.X, TraceOffset.Y, ImpactLoc.Z);
		FVector TraceEnd = TraceStart + FVector(0.0f, 0.0f, -500.0f);
		TArray<FHitResult> HitOuts;

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(this);
		CollisionQueryParams.bTraceAsyncScene = true;
		FCollisionResponseParams ResponseParams;
		ResponseParams.CollisionResponse.Visibility;

		GetWorld()->LineTraceMultiByChannel(HitOuts, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams, ResponseParams);

		for (auto HitOut : HitOuts)
		{
			ACharacter* IsCharacter = Cast<ACharacter>(HitOut.GetActor());

			if (!IsCharacter)
			{
				AWeapon* IsWeapon = Cast<AWeapon>(HitOut.GetActor());

				if (!IsWeapon)
				{
					TraceEnd = HitOut.ImpactPoint;
					BloodRot = HitOut.ImpactNormal.Rotation();
					BloodRot.Yaw = TraceNormal.Rotation().Yaw;
					break;
				}
			}
		}

		FTransform SpawnTrans;
		SpawnTrans.SetLocation(TraceEnd);
		SpawnTrans.SetRotation(FQuat(BloodRot));
		SpawnTrans.SetScale3D(FVector(1.0f, BloodSize_Decal, BloodSize_Decal));

		ABloodDecal* MyBlood = GetWorld()->SpawnActorDeferred<ABloodDecal>(BloodDecalClass, SpawnTrans, this, Instigator);
		MyBlood->BloodColor = BloodColor;
		UGameplayStatics::FinishSpawningActor(MyBlood, SpawnTrans);
	}
}

void ALibertyPrimeCharacter::DamageOverTime(float Damage, int Count)
{
	DMGOverTime = Damage;
	DMGOverCount = Count;
	DMGOverCount_Current = 0;
	GetWorldTimerManager().ClearTimer(DamageOverTimer);
	DamageOverTime_Apply();
	GetWorldTimerManager().SetTimer(DamageOverTimer, this, &ALibertyPrimeCharacter::DamageOverTime_Apply, ElementalInterval, true);
}

void ALibertyPrimeCharacter::DamageOverTime_Apply()
{
	if (!IsDead)
	{
		Health = UKismetMathLibrary::FClamp(Health - DMGOverTime, 0, MaxHealth);
		UpdateHealth_PB();
		DMGOverCount_Current += 1;

		if (Health <= 0.0f)
		{
			GetWorldTimerManager().ClearTimer(DamageOverTimer);
			Die();
		}

		if (DMGOverCount_Current >= DMGOverCount)
		{
			GetWorldTimerManager().ClearTimer(DamageOverTimer);
		}
	}
}

void ALibertyPrimeCharacter::Die()
{
	Die_Add();
	IsDead = true;
	restriction = ERestriction::Restriction_Full;
	ResetDamager();

	TArray<FLinearColor> DeathColors;
	DeathColors.Add(FLinearColor(33.000000, 4.269172, 1.969064, 1.000000));
	DeathColors.Add(FLinearColor(3.093750, 11.266720, 33.000000, 1.000000));
	DeathColors.Add(FLinearColor(3.093750, 11.266720, 33.000000, 1.000000));
	DeathColors.Add(FLinearColor(12.316765, 33.000000, 2.865456, 1.000000));

	int rand = UKismetMathLibrary::RandomIntegerInRange(0, DeathColors.Num() - 1);
	ApplyFunction("SimDis_emissive_color", DeathColors[rand]);
	DeathLine.Play();

	SoundActive_Destroy();

	Play_SoundCue(SoundCue_Death, false);

	if (IsRigidBody)
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		GetMesh()->SetSimulatePhysics(true);
	}
	else
	{
		int rand = UKismetMathLibrary::RandomIntegerInRange(0, DeathDatas.Num() - 1);
		PlayMontage(DeathDatas[rand].TargetMontage, DeathDatas[rand].PlaySpeed, ERestriction::Restriction_Full, 0.0f);
	}

	for (auto unit : ParticleCluster)
	{
		if (unit)
		{
			unit->Deactivate();
		}
	}

	for (auto unit : WeaponCluster)
	{
		ASword* IsSword = Cast<ASword>(unit);

		if (IsSword && IsSword->SoundComponent_Weapon)
		{
			SoundComponents.Add(IsSword->SoundComponent_Weapon);
		}

		unit->Clear_PS();
	}
}

void ALibertyPrimeCharacter::Die_Add()
{
	GetCapsuleComponent()->DestroyComponent();
}

void ALibertyPrimeCharacter::PlayMontage(UAnimMontage* TargetMontage, float PlaySpeed, ERestriction restriction, float TargetRotSpeed)
{
	GetMesh()->GetAnimInstance()->Montage_Play(TargetMontage, PlaySpeed);
	GetCharacterMovement()->StopMovementImmediately();
	RotLerpSpeed = TargetRotSpeed;
	this->restriction = restriction;
	EndFly();
	PlayMontage_Add();
}

void ALibertyPrimeCharacter::PlayMontage_Add()
{

}

void ALibertyPrimeCharacter::ResetMontage()
{
	this->restriction = ERestriction::Restriction_None;
	IsInvincible = false;
	ResetSequenceCount();
	ResetMontage_Add();
	RotLerpSpeed = RotLerpSpeedNormal;
}

void ALibertyPrimeCharacter::ResetMontage_Add()
{
	IsRigidBody = false;
}

void ALibertyPrimeCharacter::ActiveSensers_Add(ASword* TargetSword)
{
	if (TargetSword)
	{
		ActiveSensers.AddUnique(TargetSword);
	}
}

void ALibertyPrimeCharacter::ResetDamager(ASword* TargetSenser)
{
	if (TargetSenser)
	{
		TargetSenser->ResetDamagedChars();
		ActiveSensers.Remove(TargetSenser);
	}
}

void ALibertyPrimeCharacter::ResetDamager()
{
	for (auto ActiveSenser : ActiveSensers)
	{
		if (ActiveSenser)
		{
			ActiveSenser->ResetDamagedChars();
			ActiveSenser->EndTrail();
		}
	}

	ActiveSensers.Empty();
	ResetDamager_Add();
}

void ALibertyPrimeCharacter::ResetDamager_Add()
{

}

bool ALibertyPrimeCharacter::CheckRestriction()
{
	if (!IsDead && restriction == ERestriction::Restriction_None)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ALibertyPrimeCharacter::Freeze(float Multiplier)
{
	FireReset();
	FireLine.Reverse();
	IceLine.Play();
	FreezeTotal = Multiplier;
	CheckDilation();
	GetWorldTimerManager().ClearTimer(FreezeTimer);
	GetWorldTimerManager().SetTimer(FreezeTimer, this, &ALibertyPrimeCharacter::FreezeReset, FreezeDuration, false);
}

void ALibertyPrimeCharacter::FreezeReset()
{
	FreezeTotal = 0.0f;
	CheckDilation();
	IceLine.Reverse();
}

void ALibertyPrimeCharacter::CheckDilation()
{
	float lerpSpeed = 0.01;
	float TargetDilation = SpeedUpTotal + 1.0f - FreezeTotal;

	GetWorldTimerManager().ClearTimer(DilationTimer);

	if (CustomTimeDilation < TargetDilation)
	{
		CustomTimeDilation = UKismetMathLibrary::FClamp(CustomTimeDilation + lerpSpeed, CustomTimeDilation, TargetDilation);
	}
	else if (CustomTimeDilation > TargetDilation)
	{
		CustomTimeDilation = UKismetMathLibrary::FClamp(CustomTimeDilation - lerpSpeed, TargetDilation, CustomTimeDilation);
	}

	if (CustomTimeDilation != TargetDilation)
	{
		GetWorldTimerManager().SetTimer(DilationTimer, this, &ALibertyPrimeCharacter::CheckDilation, 0.02f, false);
	}
}

void ALibertyPrimeCharacter::Fire(int Count, float DmgVal)
{
	FreezeReset();
	GetWorldTimerManager().ClearTimer(FreezeTimer);

	FireLine.Play();
	DMGOverTime_Fire = DmgVal / 2.0f / (float)Count;
	FireCount = Count;
	FireCount_Current = 0;
	GetWorldTimerManager().ClearTimer(FireTimer);
	GetWorldTimerManager().SetTimer(FireTimer, this, &ALibertyPrimeCharacter::Fire_Apply, ElementalInterval, true);
	//DamageOverTime(DmgVal, Count);
}

void ALibertyPrimeCharacter::Fire_Apply()
{
	if (!IsDead)
	{
		Health = UKismetMathLibrary::FClamp(Health - DMGOverTime_Fire, 0, MaxHealth);

		if (PlayerController)
		{
			PlayerController->Add_Indicator(DMGOverTime_Fire, FireColor, GetActorLocation());
		}

		UpdateHealth_PB();
		FireCount_Current += 1;

		if (Health <= 0.0f)
		{
			Die();
		}

		if (FireCount_Current >= FireCount)
		{
			FireReset();
		}
	}
}

void ALibertyPrimeCharacter::FireReset()
{
	GetWorldTimerManager().ClearTimer(FireTimer);
	FireLine.Reverse();
}

void ALibertyPrimeCharacter::Poison_Clear()
{
	GetWorldTimerManager().ClearTimer(PoisonTimer);
	PoisonLine.Reverse();
}

void ALibertyPrimeCharacter::Poison(int Count, float DmgVal)
{
	PoisonLine.Play();
	DMGOverTime_Poison = DmgVal;
	PoisonCount = Count;
	PoisonCount_Current = 0;
	GetWorldTimerManager().ClearTimer(PoisonTimer);
	GetWorldTimerManager().SetTimer(PoisonTimer, this, &ALibertyPrimeCharacter::Poison_Apply, ElementalInterval, true);
}

void ALibertyPrimeCharacter::Poison_Apply()
{
	if (!IsDead)
	{
		Health = UKismetMathLibrary::FClamp(Health - DMGOverTime_Poison, 0, MaxHealth);

		if (PlayerController)
		{
			PlayerController->Add_Indicator(DMGOverTime_Poison, PoisonColor, GetActorLocation());
		}

		UpdateHealth_PB();
		PoisonCount_Current += 1;

		if (Health <= 0.0f)
		{
			Die();
		}

		if (PoisonCount_Current >= PoisonCount)
		{
			Poison_Clear();
		}
	}
}

void ALibertyPrimeCharacter::StartRot()
{
	restriction = ERestriction::Restriction_Rot;
}

void ALibertyPrimeCharacter::EndRot()
{
	restriction = ERestriction::Restriction_Full;
}

void ALibertyPrimeCharacter::MassDamage(float radius, UAttackData* AttackData, FVector TraceLoc, EWeaponType WeaponType, AActor* Source, bool bPassDMGType, EDamageType DMGType_Param)
{
	TArray<FHitResult> HitOuts;
	FVector TraceStart = TraceLoc;
	FVector TraceEnd = TraceStart + FVector(0.0f, 0.0f, 15.0f);
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALibertyPrimeCharacter::StaticClass(), FoundActors);

	for (auto FoundActor : FoundActors)
	{
		ALibertyPrimeCharacter* IsLPC = Cast<ALibertyPrimeCharacter>(FoundActor);

		if (IsLPC)
		{
			if (!IsLPC->IsDead && !IsLPC->IsInvincible && IsLPC->faction != faction)
			{
				FoundActors.Remove(IsLPC);
			}
		}
	}

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActors(FoundActors);
	CollisionQueryParams.bTraceAsyncScene = true;

	GetWorld()->SweepMultiByObjectType(HitOuts, TraceStart, TraceEnd, FQuat(), ObjectQueryParams, FCollisionShape::MakeSphere(radius), CollisionQueryParams);

	TArray<AActor*> DamagedChars;

	for (auto HitOut : HitOuts)
	{
		ALibertyPrimeCharacter* IsTarget = Cast<ALibertyPrimeCharacter>(HitOut.GetActor());

		if (IsTarget)
		{
			bool IsFound = false;

			for (auto DamagedChar : DamagedChars)
			{
				if (IsTarget == DamagedChar)
				{
					IsFound = true;
				}
			}

			if (!IsFound)
			{
				FDamageData BakedData;

				EDamageType DMGType_Target;

				if (bPassDMGType)
				{
					DMGType_Target = DMGType_Param;
				}
				else
				{
					DMGType_Target = DMGType;
				}

				if (Source != nullptr)
				{
					BakedData = BakeDamageData(Source, AttackData, DMGType_Target, WeaponType);
				}
				else
				{
					BakedData = BakeDamageData(this, AttackData, DMGType_Target, WeaponType);
				}

				IsTarget->Damager(BakedData, IsTarget->GetActorLocation());
				DamagedChars.AddUnique(IsTarget);
			}
		}
	}
}

void ALibertyPrimeCharacter::Light_SpeedUp()
{
	SpeedUpTotal = UKismetMathLibrary::FClamp(SpeedUpTotal + SpeedUpAmount, 0.0f, SpeedUpMax);
	CheckDilation();
	LightningLine.Play();
	GetWorldTimerManager().ClearTimer(LightningTimer);
	GetWorldTimerManager().SetTimer(LightningTimer, this, &ALibertyPrimeCharacter::Light_SpeedDown, 15.0f, false);
}

void ALibertyPrimeCharacter::Light_SpeedDown()
{
	LightningLine.Reverse();
	SpeedUpTotal = 0.0f;
	CheckDilation();
}

void ALibertyPrimeCharacter::ApplyElemental(ALibertyPrimeCharacter* Applier, EDamageType DMGType)
{
	switch (DMGType)
	{
	case EDamageType::Damage_Fire:
	{
		Fire(Applier->FireTick, Calculate_Effect(Applier->FireDMG, FireResist_DMG));
		break;
	}
	case EDamageType::Damage_Freeze:
	{
		Freeze(Calculate_Effect(Applier->FreezeAmount, FreezeResist_Frostbite));
		break;
	}
	case EDamageType::Damage_Poison:
	{
		Poison(Applier->PoisonTick, Calculate_Effect(Applier->PoisonDMG, PoisonResist_DMG));
		break;
	}
	default:
		break;
	}
}

float ALibertyPrimeCharacter::Calculate_Effect(float effect_val, float resist_val)
{
	float target_val = 0.0f;

	if (effect_val > 0.0f)
	{
		if (resist_val > 0.0f)
		{
			target_val = FMath::Clamp(effect_val - resist_val, 0.0f, effect_val);
		}
		else
		{
			target_val = effect_val;
		}
	}

	return target_val;
}

void ALibertyPrimeCharacter::RandomAttack()
{
	int rand = UKismetMathLibrary::RandomIntegerInRange(0, RandomAttacks.Num() - 1);

	if (RandomAttacks[rand])
	{
		Attack(RandomAttacks[rand]);
	}
}

UAudioComponent* ALibertyPrimeCharacter::Play_SoundCue(USoundCue* TargetSoundCue, bool ClearClustered, FName AttachName, float volume, USceneComponent* Attach_Comp)
{
	USceneComponent* Attach_Comp_Local;

	if (Attach_Comp == nullptr)
	{
		Attach_Comp_Local = GetMesh();
	}
	else
	{
		Attach_Comp_Local = Attach_Comp;
	}

	UAudioComponent* AudioComp_Local = UGameplayStatics::SpawnSoundAttached(TargetSoundCue, Attach_Comp_Local, AttachName, Attach_Comp_Local->GetComponentToWorld().GetLocation(), EAttachLocation::KeepWorldPosition, false, volume);

	if (ClearClustered)
	{
		SoundActive_Destroy();
		SoundComponent_Active = AudioComp_Local;
	}

	return AudioComp_Local;
}

UAudioComponent* ALibertyPrimeCharacter::Play_SoundCue_Location(USoundCue* TargetSoundCue, FVector TargetLoc, float volume, bool ClearClustered)
{
	UAudioComponent* AudioComp_Local = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), TargetSoundCue, TargetLoc, FRotator::ZeroRotator, volume);

	if (ClearClustered)
	{
		SoundActive_Destroy();
		SoundComponent_Active = AudioComp_Local;
	}

	return AudioComp_Local;
}

UAudioComponent* ALibertyPrimeCharacter::Play_SoundCue_2D(USoundCue* TargetSoundCue, float volume)
{
	UAudioComponent* AudioComp_Local = UGameplayStatics::SpawnSound2D(GetWorld(), TargetSoundCue, volume);
	return AudioComp_Local;
}

void ALibertyPrimeCharacter::SoundActive_Destroy()
{
	if (SoundComponent_Active)
	{
		SoundComponents.Add(SoundComponent_Active);
	}
}

void ALibertyPrimeCharacter::UpdateHealth_PB_Implementation()
{
	
}

void ALibertyPrimeCharacter::HealthBar_Show_Implementation()
{

}

void ALibertyPrimeCharacter::HealthBar_Hide_Implementation()
{

}

void ALibertyPrimeCharacter::PoisonFunction(float Val)
{
	ApplyFunction(FName("Poison_Param"), Val);
}

void ALibertyPrimeCharacter::IceFunction(float Val)
{
	ApplyFunction(FName("Ice_Param"), Val);
}

void ALibertyPrimeCharacter::FireFunction(float Val)
{
	ApplyFunction(FName("Fire_Param"), Val);
}

void ALibertyPrimeCharacter::LightningFunction(float Val)
{
	ApplyFunction(FName("Lightning"), Val);
}

void ALibertyPrimeCharacter::FresnelFunction(float Val)
{
	ApplyFunction(FName("Fresnel_Param"), Val);
}

void ALibertyPrimeCharacter::DeathFunction(float Val)
{
	ApplyFunction(FName("SimDis_dissolve_amount"), Val);

	if (Val >= 0.99f)
	{
		for (auto unit : WeaponCluster)
		{
			unit->Destroy();
		}

		Destroy();
	}
}

void ALibertyPrimeCharacter::ApplyFunction(FName ParamName, float Val)
{
	for (auto BodyPart : BodyParts_SK)
	{
		for (int i = 0; i < BodyPart->GetNumMaterials(); i++)
		{

			UMaterialInstanceDynamic* DynamicInstance = BodyPart->CreateAndSetMaterialInstanceDynamic(i);
			DynamicInstance->SetScalarParameterValue(ParamName, Val);
		}
	}

	for (auto BodyPart : BodyParts_ST)
	{
		for (int i = 0; i < BodyPart->GetNumMaterials(); i++)
		{

			UMaterialInstanceDynamic* DynamicInstance = BodyPart->CreateAndSetMaterialInstanceDynamic(i);
			DynamicInstance->SetScalarParameterValue(ParamName, Val);
		}
	}
}

void ALibertyPrimeCharacter::ApplyFunction(FName ParamName, FLinearColor Val)
{
	for (auto BodyPart : BodyParts_SK)
	{
		for (int i = 0; i < BodyPart->GetNumMaterials(); i++)
		{

			UMaterialInstanceDynamic* DynamicInstance = BodyPart->CreateAndSetMaterialInstanceDynamic(i);
			DynamicInstance->SetVectorParameterValue(ParamName, Val);
		}
	}

	for (auto BodyPart : BodyParts_ST)
	{
		for (int i = 0; i < BodyPart->GetNumMaterials(); i++)
		{

			UMaterialInstanceDynamic* DynamicInstance = BodyPart->CreateAndSetMaterialInstanceDynamic(i);
			DynamicInstance->SetVectorParameterValue(ParamName, Val);
		}
	}
}

void ALibertyPrimeCharacter::InitializeFunction(FTimeline* Timeline, FName functionName, UCurveFloat* CurveFloat)
{
	FOnTimelineFloat TLProgress;
	TLProgress.BindUFunction(this, functionName);
	Timeline->AddInterpFloat(CurveFloat, TLProgress);
	Timeline->SetLooping(false);
}

void ALibertyPrimeCharacter::TL_CheckTick(FTimeline* Timeline, float DeltaTime)
{
	if (Timeline->IsPlaying())
	{
		Timeline->TickTimeline(DeltaTime);
	}
}

void ALibertyPrimeCharacter::RandomAttackData_Create(FString path, TArray<UAttackData*> &AttackData_arr_ref, USoundCue* TargetSoundCue, ERestriction restriction, EImpact impact, float DMGMultiplier, float RotSpeed, float PlaySpeed, bool IsSequencer)
{
	UAttackData* AttackData_Inst = NewObject<UAttackData>(this, UAttackData::StaticClass());
	FStringAssetReference AttackMontageRef(path);
	AttackData_Inst->restriction = restriction;
	AttackData_Inst->impact = impact;
	AttackData_Inst->DMGMultiplier = DMGMultiplier;
	AttackData_Inst->RotSpeed = RotSpeed;
	AttackData_Inst->PlaySpeed = PlaySpeed;
	AttackData_Inst->IsSequencer = IsSequencer;
	AttackData_Inst->AttackSoundCue = TargetSoundCue;
	UAnimMontage* AttackMontage = Cast<UAnimMontage>(AttackMontageRef.TryLoad());
	AttackData_Inst->AttackMontage = AttackMontage;

	AttackData_arr_ref.Add(AttackData_Inst);
}

void ALibertyPrimeCharacter::RandomAttackData_Create(TArray<FString> &paths, USoundCue* TargetSoundCue, ERestriction restriction, EImpact impact, float DMGMultiplier, float RotSpeed, float PlaySpeed, bool IsSequencer)
{
	for (auto path : paths)
	{
		UAttackData* AttackData_Inst = NewObject<UAttackData>(this, UAttackData::StaticClass());
		FStringAssetReference AttackMontageRef(path);
		AttackData_Inst->restriction = restriction;
		AttackData_Inst->impact = impact;
		AttackData_Inst->DMGMultiplier = DMGMultiplier;
		AttackData_Inst->RotSpeed = RotSpeed;
		AttackData_Inst->PlaySpeed = PlaySpeed;
		AttackData_Inst->IsSequencer = IsSequencer;
		AttackData_Inst->AttackSoundCue = TargetSoundCue;
		UAnimMontage* AttackMontage = Cast<UAnimMontage>(AttackMontageRef.TryLoad());
		AttackData_Inst->AttackMontages.Add(AttackMontage);

		RandomAttacks.Add(AttackData_Inst);
	}

	paths.Empty();
}

void ALibertyPrimeCharacter::RandomAttackData_Create(FString path, USoundCue* TargetSoundCue, ERestriction restriction, EImpact impact, float DMGMultiplier, float RotSpeed, float PlaySpeed, bool IsSequencer)
{
	UAttackData* AttackData_Inst = NewObject<UAttackData>(this, UAttackData::StaticClass());
	FStringAssetReference AttackMontageRef(path);
	AttackData_Inst->restriction = restriction;
	AttackData_Inst->impact = impact;
	AttackData_Inst->DMGMultiplier = DMGMultiplier;
	AttackData_Inst->RotSpeed = RotSpeed;
	AttackData_Inst->PlaySpeed = PlaySpeed;
	AttackData_Inst->IsSequencer = IsSequencer;
	AttackData_Inst->AttackSoundCue = TargetSoundCue;
	UAnimMontage* AttackMontage = Cast<UAnimMontage>(AttackMontageRef.TryLoad());
	AttackData_Inst->AttackMontage = AttackMontage;

	RandomAttacks.Add(AttackData_Inst);
}

UAttackData* ALibertyPrimeCharacter::AttackData_Create(FString path, USoundCue* TargetSoundCue, ERestriction restriction, EImpact impact, float DMGMultiplier, float RotSpeed, float PlaySpeed, bool IsSequencer)
{
	UAttackData* AttackData_Inst = NewObject<UAttackData>(this, UAttackData::StaticClass());
	FStringAssetReference AttackMontageRef(path);
	AttackData_Inst->restriction = restriction;
	AttackData_Inst->impact = impact;
	AttackData_Inst->DMGMultiplier = DMGMultiplier;
	AttackData_Inst->RotSpeed = RotSpeed;
	AttackData_Inst->PlaySpeed = PlaySpeed;
	AttackData_Inst->IsSequencer = IsSequencer;
	AttackData_Inst->AttackSoundCue = TargetSoundCue;
	UAnimMontage* AttackMontage = Cast<UAnimMontage>(AttackMontageRef.TryLoad());
	AttackData_Inst->AttackMontage = AttackMontage;

	return AttackData_Inst;
}

UAttackData* ALibertyPrimeCharacter::AttackData_Create(USoundCue* TargetSoundCue, EImpact impact, float DMGMultiplier)
{
	UAttackData* AttackData_Inst = NewObject<UAttackData>(this, UAttackData::StaticClass());
	AttackData_Inst->impact = impact;
	AttackData_Inst->DMGMultiplier = DMGMultiplier;
	AttackData_Inst->AttackSoundCue = TargetSoundCue;

	return AttackData_Inst;
}

FDamageData ALibertyPrimeCharacter::BakeDamageData(AActor* Source, UAttackData* AttackData, EDamageType DMGType, EWeaponType WeaponType)
{
	UAttackData* AttackData_Inst = NewObject<UAttackData>(this, UAttackData::StaticClass());
	AttackData_Inst->impact = AttackData->impact;
	AttackData_Inst->DMGMultiplier = AttackData->DMGMultiplier;

	FDamageData DamageData;
	DamageData.AttackData = AttackData_Inst;
	DamageData.Source = Source;
	DamageData.DMGType = DMGType;
	DamageData.Applier = this;
	DamageData.WeaponType = WeaponType;

	return DamageData;
}

void ALibertyPrimeCharacter::RotLerp(float DeltaTime)
{
	if ((bFocus || restriction == ERestriction::Restriction_Rot || restriction == ERestriction::Restriction_Move) && !IsDead)
	{
		FRotator NewRot = UKismetMathLibrary::RInterpTo(GetActorRotation(), LerpRot, DeltaTime, RotLerpSpeed * CustomTimeDilation);
		SetActorRotation(FQuat(FRotator(0.0f, NewRot.Yaw, 0.0f)));
	}
	else if (restriction == ERestriction::Restriction_Fixed_Target)
	{
		FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FixedTarget->GetActorLocation());
		FRotator NewRot = UKismetMathLibrary::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, RotLerpSpeed * CustomTimeDilation);
		SetActorRotation(FQuat(FRotator(0.0f, NewRot.Yaw, 0.0f)));
	}
	else if (restriction == ERestriction::Restriction_Fixed_Rot)
	{
		FRotator NewRot = UKismetMathLibrary::RInterpTo(GetActorRotation(), FixedRot, DeltaTime, RotLerpSpeed * CustomTimeDilation);
		SetActorRotation(FQuat(FRotator(0.0f, NewRot.Yaw, 0.0f)));
	}
}

void ALibertyPrimeCharacter::Walk_Start()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ALibertyPrimeCharacter::Walk_Stop()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void ALibertyPrimeCharacter::Breathe()
{
	if (!IsDead)
	{
		if (!SoundComponent_Active->IsValidLowLevel())
		{
			Play_SoundCue(SoundCue_Breathe);
		}

		GetWorldTimerManager().SetTimer(BreatheTimer, this, &ALibertyPrimeCharacter::Breathe, BreatheInterval, false);
	}
}

void ALibertyPrimeCharacter::Play_FootStep()
{
	if (CanPlayFootstep)
	{
		CanPlayFootstep = false;

		FVector Foot_Offset = FVector(0.0f, 0.0f, Cap_Height);
		FVector TraceStart = GetActorLocation() + Foot_Offset;
		FVector TraceEnd = TraceStart - (Foot_Offset * 3.0f);

		FHitResult HitOut;
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.bReturnPhysicalMaterial = true;
		CollisionQueryParams.AddIgnoredActor(this);

		GetWorld()->LineTraceSingleByChannel(HitOut, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);

		if (HitOut.PhysMaterial != NULL)
		{
			switch (HitOut.PhysMaterial->SurfaceType)
			{
			case EPhysicalSurface::SurfaceType1:
				Play_SoundCue_Location(Footstep_Concrete, HitOut.ImpactPoint, FootstepVolume);
				break;
			case EPhysicalSurface::SurfaceType2:
				Play_SoundCue_Location(Footstep_Dirt, HitOut.ImpactPoint, FootstepVolume);
				break;
			case EPhysicalSurface::SurfaceType3:
				Play_SoundCue_Location(Footstep_Grass, HitOut.ImpactPoint, FootstepVolume);
				break;
			case EPhysicalSurface::SurfaceType4:
				Play_SoundCue_Location(Footstep_Gravel, HitOut.ImpactPoint, FootstepVolume);
				break;
			case EPhysicalSurface::SurfaceType5:
				Play_SoundCue_Location(Footstep_Snow, HitOut.ImpactPoint, FootstepVolume);
				break;
			case EPhysicalSurface::SurfaceType6:
				Play_SoundCue_Location(Footstep_Mud, HitOut.ImpactPoint, FootstepVolume);
				break;
			case EPhysicalSurface::SurfaceType7:
				Play_SoundCue_Location(Footstep_Metal, HitOut.ImpactPoint, FootstepVolume);
				break;
			case EPhysicalSurface::SurfaceType8:
				Play_SoundCue_Location(Footstep_Water, HitOut.ImpactPoint, FootstepVolume);
				break;
			case EPhysicalSurface::SurfaceType9:
				Play_SoundCue_Location(Footstep_Sand, HitOut.ImpactPoint, FootstepVolume);
				break;
			case EPhysicalSurface::SurfaceType10:
				Play_SoundCue_Location(Footstep_Wood, HitOut.ImpactPoint, FootstepVolume);
				break;
			default:
				 (Footstep_Concrete, HitOut.ImpactPoint);
				break;
			}
		}

		GetWorldTimerManager().SetTimer(FootstepTimer, this, &ALibertyPrimeCharacter::FootStep_Reset, FootStepInterval, false);
	}
}

void ALibertyPrimeCharacter::FootStep_Reset()
{
	CanPlayFootstep = true;
}

void ALibertyPrimeCharacter::StartFly()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
}

void ALibertyPrimeCharacter::EndFly()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void ALibertyPrimeCharacter::Build_Up()
{

}