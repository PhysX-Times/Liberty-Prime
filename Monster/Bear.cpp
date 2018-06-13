// Fill out your copyright notice in the Description page of Project Settings.

#include "Bear.h"
#include "Weapon/Sword.h"
#include "Miscellaneous/AttackData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"

ABear::ABear()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj(TEXT("AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Bear/Enemy_Bear_Animations/ExoGame_Bears_Attack_Death_Montage.ExoGame_Bears_Attack_Death_Montage'"));
	DeathDatas.Add(FMontageData(DeathMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj(TEXT("AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Bear/Enemy_Bear_Animations/Bear_GetHit_Montage.Bear_GetHit_Montage'"));
	HeavyHitDatas.Add(FMontageData(HitMontageObj.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BearMat1(TEXT("MaterialInstanceConstant'/Game/InfinityBladeAdversaries/Enemy/Enemy_Bear/Materials/M_Bear_Polar_Inst.M_Bear_Polar_Inst'"));
	BearMats.Add(BearMat1.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> BearMat2(TEXT("MaterialInstanceConstant'/Game/InfinityBladeAdversaries/Enemy/Enemy_Bear/Materials/M_Bear_Stone_Inst.M_Bear_Stone_Inst'"));
	BearMats.Add(BearMat2.Object);
}

void ABear::BeginPlay()
{
	Super::BeginPlay();

	int rand = UKismetMathLibrary::RandomIntegerInRange(0, BearMats.Num() - 1);
	GetMesh()->SetMaterial(0, BearMats[rand]);

	FString RefPath;

	RefPath = "AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Bear/Enemy_Bear_Animations/Bear_AttackLeft_Montage.Bear_AttackLeft_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 0.8f, false);

	RefPath = "AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Bear/Enemy_Bear_Animations/Bear_AttackRight_Montage.Bear_AttackRight_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 0.8f, false);

	RefPath = "AnimMontage'/Game/InfinityBladeAdversaries/Enemy/Enemy_Bear/Enemy_Bear_Animations/Bear_AttackBoth_Montage.Bear_AttackBoth_Montage'";
	RandomAttackData_Create(RefPath, SoundCue_Attack_Medium, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.2f, 0.8f, false);

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