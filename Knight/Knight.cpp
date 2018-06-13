// Fill out your copyright notice in the Description page of Project Settings.

#include "Knight.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"
#include "Miscellaneous/AttackData.h"
#include "KnightColor.h"
#include "Weapon/Sword.h"
#include "Engine.h"


AKnight::AKnight()
{
	RunSpeed = 600.0f;
	WalkSpeed = 150.0f;
	AttackDistance = 197.5f;
	BloodSize_Decal = 0.6f;

	bSpawnBlood = true;
	CanBeHeavyHit = true;
	CanHeavyHitRot = true;
	CanBeNormalHit = true;
	bHeavyInvincible = true;
	bHeavyRigidBody = true;

	montage_stop_cond = EICondition::Condition_None;
	push_cond = EICondition::Condition_Normal;

	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(GetMesh());
	Breastplate = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Breastplate"));
	Breastplate->SetupAttachment(GetMesh());
	Cape = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Cape"));
	Cape->SetupAttachment(GetMesh());
	Hood = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hood"));
	Hood->SetupAttachment(GetMesh());
	Arm = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arm"));
	Arm->SetupAttachment(GetMesh());
	Belt = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Belt"));
	Belt->SetupAttachment(GetMesh());
	WaistSide = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WaistSide"));
	WaistSide->SetupAttachment(GetMesh());
	WaistBack = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WaistBack"));
	WaistBack->SetupAttachment(GetMesh());
	ClothFront = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ClothFront"));
	ClothFront->SetupAttachment(GetMesh());
	ClothBack = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ClothBack"));
	ClothBack->SetupAttachment(GetMesh());
	Leg = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Leg"));
	Leg->SetupAttachment(GetMesh());
	LegDeco = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegDeco"));
	LegDeco->SetupAttachment(GetMesh());

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj1(TEXT("/Game/ModularKnight/Animations/DynamicSwordAnimset/Animations/InPlace/HitMontage01"));
	HitDatas.Add(FMontageData(HitMontageObj1.Object, 0.75f));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj2(TEXT("/Game/ModularKnight/Animations/DynamicSwordAnimset/Animations/InPlace/HitMontage02"));
	HitDatas.Add(FMontageData(HitMontageObj2.Object, 0.75f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj01(TEXT("/Game/ModularKnight/Animations/DynamicSwordAnimset/Animations/RootMotion/DeadMontage01"));
	DeathDatas.Add(FMontageData(DeathMontageObj01.Object, 1.0f));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj02(TEXT("/Game/ModularKnight/Animations/DynamicSwordAnimset/Animations/RootMotion/DeadMontage02"));
	DeathDatas.Add(FMontageData(DeathMontageObj02.Object, 1.0f));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj03(TEXT("/Game/ModularKnight/Animations/DynamicSwordAnimset/Animations/RootMotion/DeadMontage03"));
	DeathDatas.Add(FMontageData(DeathMontageObj03.Object, 1.0f));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMontageObj04(TEXT("/Game/ModularKnight/Animations/DynamicSwordAnimset/Animations/RootMotion/DeadMontage04"));
	DeathDatas.Add(FMontageData(DeathMontageObj04.Object, 1.0f));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HeavyHitMontageObj01(TEXT("AnimMontage'/Game/ModularKnight/Animations/DynamicSwordAnimset/Animations/RootMotion/KnockFrontMontage.KnockFrontMontage'"));
	HeavyHitDatas.Add(FMontageData(HeavyHitMontageObj01.Object, 1.0f));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HeavyHitMontageObj02(TEXT("AnimMontage'/Game/ModularKnight/Animations/DynamicSwordAnimset/Animations/RootMotion/KnockBackMontage.KnockBackMontage'"));
	HeavyHitDatas.Add(FMontageData(HeavyHitMontageObj02.Object, 1.0f));
}

void AKnight::BeginPlay()
{
	Super::BeginPlay();

	RandomizeAppearance();

	ElementalPSLocs.Add("ElementalSocket_1");
}

void AKnight::RandomizeAppearance()
{
	if (true)
	{
		TArray<FString> RefPaths;

		int rand = 0;

		BodyParts_SK.Empty();

		RefPaths.Empty();
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Shoulderpads/ShoulderpadA/ShoulderpadA.ShoulderpadA'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Shoulderpads/ShoulderpadB/ShoulderpadB.ShoulderpadB'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Shoulderpads/ShoulderpadC/ShoulderpadC.ShoulderpadC'");
		GetMesh()->SetSkeletalMesh(LoadMesh(RefPaths));

		rand = UKismetMathLibrary::RandomIntegerInRange(1, 1);

		if (rand == 1)
		{
			GetMesh()->SetSkeletalMesh(LoadMesh(RefPaths));

			RefPaths.Empty();
			RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Cape/Cape.Cape'");
			Cape->SetSkeletalMesh(LoadMesh(RefPaths));
			BodyParts_SK.Add(Cape);
		}

		RefPaths.Empty();
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Head/HeadA/SK_HeadA.SK_HeadA'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Head/HeadB/SK_HeadB.SK_HeadB'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Head/HeadC/SK_HeadC.SK_HeadC'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Head/HeadD/SK_HeadD.SK_HeadD'");
		Head->SetSkeletalMesh(LoadMesh(RefPaths));
		BodyParts_SK.Add(Head);

		RefPaths.Empty();
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Arm/ArmA/ArmA-1/SK_Arm_A01.SK_Arm_A01'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Arm/ArmA/ArmA-2/SK_Arm_A02.SK_Arm_A02'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Arm/ArmA/ArmA-3/SK_Arm_A03.SK_Arm_A03'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Arm/ArmA/ArmA-4/SK_Arm_A04.SK_Arm_A04'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Arm/ArmA/ArmA-5/SK_Arm_A05.SK_Arm_A05'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Arm/ArmA/ArmA-6/SK_Arm_A06.SK_Arm_A06'");

		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Arm/ArmB/ArmB-1/SK_Arm_B01.SK_Arm_B01'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Arm/ArmB/ArmB-2/SK_Arm_B02.SK_Arm_B02'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Arm/ArmB/ArmB-3/SK_Arm_B03.SK_Arm_B03'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Arm/ArmB/ArmB-4/SK_Arm_B04.SK_Arm_B04'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Arm/ArmB/ArmB-5/SK_Arm_B05.SK_Arm_B05'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Arm/ArmB/ArmB-6/SK_Arm_B06.SK_Arm_B06'");

		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Arm/ArmC/ArmC-1/SK_Arm_C01.SK_Arm_C01'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Arm/ArmC/ArmC-2/SK_Arm_C02.SK_Arm_C02'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Arm/ArmC/ArmC-3/SK_Arm_C03.SK_Arm_C03'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Arm/ArmC/ArmC-4/SK_Arm_C04.SK_Arm_C04'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Arm/ArmC/ArmC-5/SK_Arm_C05.SK_Arm_C05'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Arm/ArmC/ArmC-6/SK_Arm_C06.SK_Arm_C06'");
		Arm->SetSkeletalMesh(LoadMesh(RefPaths));
		BodyParts_SK.Add(Arm);

		RefPaths.Empty();
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Breastplate/BreastplateA/BreastplateA-1/SK_Breastplate_A01.SK_Breastplate_A01'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Breastplate/BreastplateA/BreastplateA-2/SK_Breastplate_A02.SK_Breastplate_A02'");

		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Breastplate/BreastplateB/BreastplateB-1/SK_Breastplate_B01.SK_Breastplate_B01'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Breastplate/BreastplateB/BreastplateB-2/SK_Breastplate_B02.SK_Breastplate_B02'");

		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Breastplate/BreastplateC/BreastplateC-1/SK_Breastplate_C01.SK_Breastplate_C01'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Breastplate/BreastplateC/BreastplateC-2/SK_Breastplate_C02.SK_Breastplate_C02'");
		Breastplate->SetSkeletalMesh(LoadMesh(RefPaths));
		BodyParts_SK.Add(Breastplate);

		rand = UKismetMathLibrary::RandomIntegerInRange(0, 1);

		if (rand == 1)
		{
			RefPaths.Empty();
			RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Accesories/Hoods/Hood-1/Hood-1.Hood-1'");
			RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Accesories/Hoods/Hood-2/Hood-2.Hood-2'");
			Hood->SetSkeletalMesh(LoadMesh(RefPaths));
			BodyParts_SK.Add(Hood);
		}

		RefPaths.Empty();
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Accesories/Belts/BeltA/BeltA-1/BeltA-1.BeltA-1'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Accesories/Belts/BeltA/BeltA-2/BeltA-2.BeltA-2'");

		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Accesories/Belts/BeltB/BeltB-1/BeltB-1.BeltB-1'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Accesories/Belts/BeltB/BeltB-2/BeltB-2.BeltB-2'");

		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Accesories/Belts/BeltC/BeltC-1/BeltC-1.BeltC-1'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Accesories/Belts/BeltC/BeltC-2/BeltC-2.BeltC-2'");
		Belt->SetSkeletalMesh(LoadMesh(RefPaths));
		BodyParts_SK.Add(Belt);

		rand = UKismetMathLibrary::RandomIntegerInRange(0, 1);

		if (rand == 1)
		{
			RefPaths.Empty();
			RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Accesories/WaistSides/WaistA/WaistA.WaistA'");
			RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Accesories/WaistSides/WaistB/WaistB.WaistB'");
			RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Accesories/WaistSides/WaistC/WaistC.WaistC'");
			WaistSide->SetSkeletalMesh(LoadMesh(RefPaths));
			BodyParts_SK.Add(WaistSide);
		}

		rand = UKismetMathLibrary::RandomIntegerInRange(0, 1);

		if (rand == 1)
		{
			RefPaths.Empty();
			RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Accesories/BackAccessory/WraistBack/WraistBack.WraistBack'");
			WaistBack->SetSkeletalMesh(LoadMesh(RefPaths));
			BodyParts_SK.Add(WaistBack);
		}

		/*
		rand = UKismetMathLibrary::RandomIntegerInRange(0, 1);

		if (rand == 1)
		{
		RefPaths.Empty();
		ClothBack->SetSkeletalMesh(LoadMesh(RefPaths));
		BodyParts_SK.Add(ClothBack);
		}
		*/

		rand = UKismetMathLibrary::RandomIntegerInRange(0, 1);

		if (rand == 1)
		{
			RefPaths.Empty();
			RefPaths.Add("SkeletalMesh'/Game/ModularKnight/ClothAccesories/ClothA/ClothA.ClothA'");
			RefPaths.Add("SkeletalMesh'/Game/ModularKnight/ClothAccesories/ClothB/ClothB.ClothB'");
			ClothFront->SetSkeletalMesh(LoadMesh(RefPaths));
			BodyParts_SK.Add(ClothFront);
		}

		RefPaths.Empty();
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Legs/LegA/LegA-1/SK_Leg_A01.SK_Leg_A01'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Legs/LegA/LegA-2/SK_Leg_A02.SK_Leg_A02'");

		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Legs/LegB/LegB-1/SK_Leg_B01.SK_Leg_B01'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Legs/LegB/LegB-2/SK_Leg_B02.SK_Leg_B02'");

		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Legs/LegC/LegC-1/SK_Leg_C01.SK_Leg_C01'");
		RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Legs/LegC/LegC-2/SK_Leg_C02.SK_Leg_C02'");
		Leg->SetSkeletalMesh(LoadMesh(RefPaths));
		BodyParts_SK.Add(Leg);

		rand = UKismetMathLibrary::RandomIntegerInRange(0, 1);

		if (rand == 1)
		{
			RefPaths.Empty();
			RefPaths.Add("SkeletalMesh'/Game/ModularKnight/Accesories/BackAccessory/FootDeco/FootDeco.FootDeco'");
			LegDeco->SetSkeletalMesh(LoadMesh(RefPaths));
			BodyParts_SK.Add(LegDeco);
		}

		for (auto BodyPart : BodyParts_SK)
		{
			BodyPart->SetMasterPoseComponent(GetMesh());
		}

		BodyParts_SK.Add(GetMesh());

		UKnightColor* Silver = NewObject<UKnightColor>(this, UKnightColor::StaticClass());
		Silver->ArmorColor = FLinearColor(0.330000, 0.330000, 0.330000, 1.000000);
		Silver->ClothColors.Empty();
		Silver->ClothColors.Add(FLinearColor(0.124930, 0.195636, 0.220000, 1.000000));
		Silver->ClothColors.Add(FLinearColor(0.175000, 0.008750, 0.012357, 1.000000));

		UKnightColor* Steel = NewObject<UKnightColor>(this, UKnightColor::StaticClass());
		Steel->ArmorColor = FLinearColor(0.192365, 0.248580, 0.375000, 1.000000);
		Steel->ClothColors.Empty();
		Steel->ClothColors.Add(FLinearColor(0.174250, 0.250104, 0.275000, 1.000000));
		Steel->ClothColors.Add(FLinearColor(0.175000, 0.008750, 0.012357, 1.000000));

		UKnightColor* Iron = NewObject<UKnightColor>(this, UKnightColor::StaticClass());
		Iron->ArmorColor = FLinearColor(0.620000, 0.418938, 0.278990, 1.000000);
		Iron->ClothColors.Empty();
		Iron->ClothColors.Add(FLinearColor(0.275000, 0.093727, 0.063525, 1.000000));

		UKnightColor* Emerald = NewObject<UKnightColor>(this, UKnightColor::StaticClass());
		Emerald->ArmorColor = FLinearColor(0.023370, 0.110000, 0.055891, 1.000000);
		Emerald->ClothColors.Empty();
		Emerald->ClothColors.Add(FLinearColor(0.275000, 0.142932, 0.038000, 1.000000));

		UKnightColor* Gold = NewObject<UKnightColor>(this, UKnightColor::StaticClass());
		Gold->ArmorColor = FLinearColor(0.475000, 0.139477, 0.010080, 1.000000);
		Gold->ClothColors.Empty();
		Gold->ClothColors.Add(FLinearColor(0.195000, 0.092142, 0.068719, 1.000000));

		UKnightColor* Dark = NewObject<UKnightColor>(this, UKnightColor::StaticClass());
		Dark->ArmorColor = FLinearColor(0.100000, 0.100000, 0.100000, 1.000000);
		Dark->ClothColors.Empty();
		Dark->ClothColors.Add(FLinearColor(0.175000, 0.175000, 0.175000, 1.000000));

		UKnightColor* DarkBlue = NewObject<UKnightColor>(this, UKnightColor::StaticClass());
		DarkBlue->ArmorColor = FLinearColor(0.068229, 0.074480, 0.125000, 1.000000);
		DarkBlue->ClothColors.Empty();
		DarkBlue->ClothColors.Add(FLinearColor(0.040590, 0.052362, 0.075000, 1.000000));

		UKnightColor* Bronze = NewObject<UKnightColor>(this, UKnightColor::StaticClass());
		Bronze->ArmorColor = FLinearColor(0.275000, 0.155768, 0.102886, 1.000000);
		Bronze->ClothColors.Empty();
		Bronze->ClothColors.Add(FLinearColor(0.195000, 0.151669, 0.104775, 1.000000));

		TArray<UKnightColor*> Mats;
		Mats.Empty();
		Mats.Add(Silver);
		Mats.Add(Steel);
		Mats.Add(Iron);
		Mats.Add(Emerald);
		Mats.Add(Gold);
		Mats.Add(Dark);
		Mats.Add(DarkBlue);
		Mats.Add(Bronze);

		rand = UKismetMathLibrary::RandomIntegerInRange(0, Mats.Num() - 1);

		SetMeshColor(Mats[rand]);
	}
}

USkeletalMesh* AKnight::LoadMesh(TArray<FString> Paths)
{
	int rand = UKismetMathLibrary::RandomIntegerInRange(0, Paths.Num() - 1);
	FStringAssetReference BodyPartRef(Paths[rand]);
	USkeletalMesh* TargetMesh = Cast<USkeletalMesh>(BodyPartRef.TryLoad());

	return TargetMesh;
}

void AKnight::SetMeshColor(UKnightColor* TargetMat)
{
	int rand = UKismetMathLibrary::RandomIntegerInRange(0, TargetMat->ClothColors.Num() - 1);
	for (auto BodyPart : BodyParts_SK)
	{
		for (int i = 0; i < BodyPart->GetNumMaterials(); i++)
		{
			UMaterialInstanceDynamic* DynamicInstance = UMaterialInstanceDynamic::Create(BodyPart->GetMaterial(i), this);
			DynamicInstance->SetVectorParameterValue(FName("ArmorColor"), TargetMat->ArmorColor);
			DynamicInstance->SetVectorParameterValue(FName("ClothColor"), TargetMat->ClothColors[rand]);
			BodyPart->SetMaterial(i, DynamicInstance);
		}
	}
}

void AKnight::ChangeElemental(EDamageType ElementalType)
{
	MySword->ApplyEffect(ElementalType, true);
	DMGType = ElementalType;
}