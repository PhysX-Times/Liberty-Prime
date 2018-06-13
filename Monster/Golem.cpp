// Fill out your copyright notice in the Description page of Project Settings.

#include "Golem.h"
#include "Weapon/Sword.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Engine.h"

AGolem::AGolem()
{
	ConstructMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ConstructMesh"));
	ConstructMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/Enemy/Golem/Animations/GOLEM_death_Montage.GOLEM_death_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj(TEXT("AnimMontage'/Game/Enemy/Golem/Animations/GOLEM_getHitLight_Montage.GOLEM_getHitLight_Montage'"));
	HeavyHitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> Mat1(TEXT("MaterialInstanceConstant'/Game/Enemy/Golem/Materials/M_GOLEM_ROCK_Inst.M_GOLEM_ROCK_Inst'"));
	RockMats.Add(Mat1.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> Mat2(TEXT("MaterialInstanceConstant'/Game/Enemy/Golem/Materials/M_GOLEM_ICE_Inst.M_GOLEM_ICE_Inst'"));
	RockMats.Add(Mat2.Object);

	bBuildUp = true;
}

void AGolem::BeginPlay()
{
	Super::BeginPlay();

	IsInvincible = true;

	int rand = UKismetMathLibrary::RandomIntegerInRange(0, RockMats.Num() - 1);
	UMaterialInstance* Mat_Inst = RockMats[rand];
	GetMesh()->SetMaterial(0, Mat_Inst);
	ConstructMesh->SetMaterial(0, Mat_Inst);

	FString RefPath;

	RefPath = "AnimMontage'/Game/Enemy/Golem/Animations/GOLEM_2HitCombo_A_Forward_Montage.GOLEM_2HitCombo_A_Forward_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/Enemy/Golem/Animations/GOLEM_2HitCombo_C_Forward_Montage.GOLEM_2HitCombo_C_Forward_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Full, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/Enemy/Golem/Animations/GOLEM_3HitComboForward_Montage.GOLEM_3HitComboForward_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/Enemy/Golem/Animations/GOLEM_attack1Forward_Montage.GOLEM_attack1Forward_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/Enemy/Golem/Animations/GOLEM_attack3Forward_Montage.GOLEM_attack3Forward_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 1.0f, false);
	RefPath = "AnimMontage'/Game/Enemy/Golem/Animations/GOLEM_attack4Forward_Montage.GOLEM_attack4Forward_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Heavy, 2.0f, 2.2f, 1.0f, false);

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
}

void AGolem::Build_Up()
{
	ConstructMesh->GetAnimInstance()->Montage_Play(Pose_Montage, 1.0f);
	GetCharacterMovement()->StopMovementImmediately();
	restriction = ERestriction::Restriction_Full;
	Play_SoundCue_Location(SoundCue_GetPose, GetActorLocation());
}

void AGolem::Pose_End()
{
	IsInvincible = false;
	ConstructMesh->DestroyComponent();
	GetMesh()->SetVisibility(true);
	Pose_Notify();
	ResetMontage();
}

void AGolem::Smash()
{
	FVector TraceStart = GetMesh()->GetSocketLocation("SmashSocket") + FVector(0.0f, 0.0f, 50.0f);
	FVector TraceEnd = TraceStart - FVector(0.0f, 0.0f, 175.5f);
	FHitResult HitOut;

	TArray<AActor*> FoundActors;
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeapon::StaticClass(), AllActors);
	FoundActors.Append(AllActors);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALibertyPrimeCharacter::StaticClass(), AllActors);
	FoundActors.Append(AllActors);

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActors(FoundActors);
	CollisionQueryParams.bTraceAsyncScene = true;

	GetWorld()->LineTraceSingleByChannel(HitOut, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);

	if (HitOut.bBlockingHit)
	{
		FVector TargetPoint = HitOut.ImpactPoint + HitOut.ImpactNormal * 12.5f;
		AttackData->DMGMultiplier = 2.0f;
		AttackData->impact = EImpact::Impact_Heavy;
		MassDamage(275.0f, AttackData, TargetPoint, EWeaponType::Weapon_None);

		FTransform PSTrans;
		PSTrans.SetLocation(TargetPoint);
		PSTrans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
		PSTrans.SetScale3D(FVector(1.1f, 1.1f, 1.1f));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SmashPS, PSTrans, true);
		Play_SoundCue_Location(SmashCue, TargetPoint);
	}
}

void AGolem::Pose_Notify_Implementation()
{

}