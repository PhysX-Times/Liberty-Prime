// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerChar.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Miscellaneous/AttackData.h"
#include "Runtime/Landscape/Classes/Landscape.h"
#include "Runtime/Engine/Classes/PhysicalMaterials/PhysicalMaterial.h"
#include "Miscellaneous/Item.h"
#include "Weapon/Sword.h"
#include "Save/PlayerSave.h"
#include "Engine.h"
#include "Miscellaneous/Items/Inferno.h"
#include "Miscellaneous/Items/CernunnosBlessing.h"
#include "Miscellaneous/Items/VenomBlade.h"
#include "Miscellaneous/Items/VenomSerpent.h"

APlayerChar::APlayerChar()
{
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

										  // Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	AreaLightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("AreaLightComponent"));
	AreaLightComp->SetupAttachment(GetMesh(), FName("pelvis"));

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	Health = 75.0f;
	MaxHealth = 75.0f;

	faction = EFaction::Player;
	DMGType = EDamageType::Damage_None;

	bMoveForward = EMoveDirection::Move_None;
	bMoveRight = EMoveDirection::Move_None;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj1(TEXT("/Game/ModularKnight/Animations/DynamicSwordAnimset/Animations/InPlace/HitMontage01"));
	HitDatas.Add(FMontageData(HitMontageObj1.Object, 1.35f));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageObj2(TEXT("/Game/ModularKnight/Animations/DynamicSwordAnimset/Animations/InPlace/HitMontage02"));
	HitDatas.Add(FMontageData(HitMontageObj2.Object, 1.35f));

	slot_stages.Init(0, 9);
	SlotItems.Init(nullptr, 9);
}

void APlayerChar::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Attack", IE_Pressed, this, &APlayerChar::AttackFunction);
	InputComponent->BindAction("Block", IE_Pressed, this, &APlayerChar::Block_Pressed);
	InputComponent->BindAction("Block", IE_Released, this, &APlayerChar::Block_Released);
	InputComponent->BindAction("Skill-1", IE_Pressed, this, &APlayerChar::DashAttack);
	InputComponent->BindAction("Skill-2", IE_Pressed, this, &APlayerChar::SpinAttack);
	InputComponent->BindAction("Skill-3", IE_Pressed, this, &APlayerChar::EnergyAttack);
	InputComponent->BindAction("Skill-4", IE_Pressed, this, &APlayerChar::IronWill_Apply);

	InputComponent->BindAction("Inventory", IE_Pressed, this, &APlayerChar::LoadInventory);

	InputComponent->BindAction("Save", IE_Pressed, this, &APlayerChar::Save);
	InputComponent->BindAction("Load", IE_Pressed, this, &APlayerChar::Load);

	InputComponent->BindAction("Dodge", IE_Pressed, this, &APlayerChar::Dodge);

	InputComponent->BindAction("Fire", IE_Pressed, this, &APlayerChar::ApplyFire);
	InputComponent->BindAction("Ice", IE_Pressed, this, &APlayerChar::ApplyIce);
	InputComponent->BindAction("Poison", IE_Pressed, this, &APlayerChar::ApplyPoison);
	InputComponent->BindAction("Lightning", IE_Pressed, this, &APlayerChar::ApplyLightning);
	InputComponent->BindAction("None", IE_Pressed, this, &APlayerChar::ApplyNone);


	InputComponent->BindAction("Walk", IE_Pressed, this, &APlayerChar::Walk_Start);
	InputComponent->BindAction("Walk", IE_Released, this, &APlayerChar::Walk_Stop);

	InputComponent->BindAxis("MoveForward", this, &APlayerChar::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight);
}

void APlayerChar::IronWill_Apply()
{
	if (CheckRestriction())
	{
		IronWill_Start();
		GetWorldTimerManager().SetTimer(IronWillTimer, this, &ALibertyPrimeCharacter::IronWill_End, 10.0f, false);
	}
}

void APlayerChar::AttackFunction()
{
	if (!IsDead && !DidAttack)
	{
		if (AttackSequenceCount != 0)
		{
			DidAttack = true;
		}
		else
		{
			if (CheckRestriction())
			{
				Attack(AttackData_const);
			}
		}
	}
}

void APlayerChar::AttackCheck()
{
	if (!DidAttack)
	{
		GetMesh()->GetAnimInstance()->Montage_Stop(0.25f);
		ResetMontage();
	}
	else
	{
		DidAttack = false;
	}
}

void APlayerChar::AttackSequenceCheck()
{
	if (DidAttack)
	{
		DidAttack = false;
		Attack(AttackData);
	}
}

void APlayerChar::BeginPlay()
{
	Super::BeginPlay();

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
	SpawnTrans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));

	FString RefPath;
	RefPath = "AnimMontage'/Game/ModularKnight/Animations/DynamicSwordAnimset/Animations/RootMotion/DashAttackMontage.DashAttackMontage'";
	DashAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Heavy, 1.75f, 4.7f, 1.0f, false);

	RefPath = "AnimMontage'/Game/ModularKnight/Animations/DynamicSwordAnimset/Animations/RootMotion/SpinAttackMontage.SpinAttackMontage'";
	SpinAttackData = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 4.7f, 1.0f, false);

	RefPath = "AnimMontage'/Game/ModularKnight/Animations/DynamicSwordAnimset/Animations/RootMotion/AttackDynamicDouble.AttackDynamicDouble'";
	AttackData_const = AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Rot, EImpact::Impact_Normal, 1.0f, 2.7f, 1.0f, false);

	RefPath = "AnimMontage'/Game/ModularKnight/Animations/SnSAnimsetPro/Animations/RootMotion/SnSCounter_Whirlwind.SnSCounter_Whirlwind'";
	CounterDatas.Add(AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Fixed_Target, EImpact::Impact_Normal, 1.0f, 7.7f, 1.27f, false));
	RefPath = "AnimMontage'/Game/ModularKnight/Animations/SnSAnimsetPro/Animations/RootMotion/SnSCounter_L.SnSCounter_L'";
	CounterDatas.Add(AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Fixed_Target, EImpact::Impact_Normal, 1.0f, 7.7f, 1.27f, false));
	RefPath = "AnimMontage'/Game/ModularKnight/Animations/SnSAnimsetPro/Animations/RootMotion/SnSCounter_R.SnSCounter_R'";
	CounterDatas.Add(AttackData_Create(RefPath, SoundCue_Attack, ERestriction::Restriction_Fixed_Target, EImpact::Impact_Normal, 1.0f, 7.7f, 1.27f, false));

	MySword = GetWorld()->SpawnActorDeferred<ASword>(SwordClass, SpawnTrans, this, Instigator);
	MySword->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(MySword, SpawnTrans);
	MySword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("SwordSocket"));

	WeaponCluster.Add(MySword);

	FStringAssetReference CounterCurveFloatRef("CurveFloat'/Game/TopDownCPP/Blueprints/CounterCurve.CounterCurve'");
	CounterCurveFloat = Cast<UCurveFloat>(CounterCurveFloatRef.TryLoad());
	InitializeFunction(&CounterLine, FName("CounterFunction"), CounterCurveFloat);

	for (auto BodyPart_SK : BodyParts_SK)
	{
		BodyPart_SK->SetRenderCustomDepth(true);
	}

	slot_stages = { 3, 3, 3, 2, 2, 1, 1, 0, 0 };

	InventoryWidget = CreateWidget<UUserWidget>(GetWorld(), InventoryWidget_Class);
	PlayerWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerWidget_Class);
	FInputModeGameAndUI inputMode;
	inputMode.SetHideCursorDuringCapture(false);
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(inputMode);
	GEngine->GameViewport->Viewport->LockMouseToViewport(true);
	ChangeWidget(PlayerWidget, false);

	Widget_Load();
	Inventory_Items_Load();

	for (auto InventoryItem : InventoryItems)
	{
		InventoryItem->MyOwner = this;
		InventoryItem->Apply_Effect();
	}

	TArray<TSubclassOf<UItem>> ItemClasses;

	for (TObjectIterator<UClass> It; It; ++It)
	{
		if (It->IsChildOf(UItem::StaticClass()) && !It->HasAnyClassFlags(CLASS_Abstract) && *It != UItem::StaticClass())
		{
			ItemClasses.Add(*It);
		}
	}

	InventoryItems.Empty();

	for (auto ItemClass : ItemClasses)
	{
		UItem* Item_Inst = NewObject<UItem>(this, ItemClass);
		Item_Inst->MyOwner = this;
		Add_Item(Item_Inst);
	}

	UItem* Item_Inst = NewObject<UItem>(this, UCernunnosBlessing::StaticClass());
	Item_Inst->MyOwner = this;
	Add_Item(Item_Inst);

	Item_Inst = NewObject<UItem>(this, UInferno::StaticClass());
	Item_Inst->MyOwner = this;
	Add_Item(Item_Inst);

	Item_Inst = NewObject<UItem>(this, UVenomSerpent::StaticClass());
	Item_Inst->MyOwner = this;
	Add_Item(Item_Inst);

	Item_Inst = NewObject<UItem>(this, UInferno::StaticClass());
	Item_Inst->MyOwner = this;
	Add_Item(Item_Inst);

	Item_Inst = NewObject<UItem>(this, UVenomSerpent::StaticClass());
	Item_Inst->MyOwner = this;
	Add_Item(Item_Inst);

	/*
	UPlayerSave* PlayerSave_Inst = Cast<UPlayerSave>(UGameplayStatics::CreateSaveGameObject(UPlayerSave::StaticClass()));
	PlayerSave_Inst = Cast<UPlayerSave>(UGameplayStatics::LoadGameFromSlot(PlayerSave_Inst->SaveSlotName, PlayerSave_Inst->UserIndex));

	for (auto itemClass : PlayerSave_Inst->ItemClass)
	{
		UItem* Item_Inst = NewObject<UItem>(this, itemClass);
		Item_Inst->MyOwner = this;
		Item_Inst->Apply_Effect();
		InventoryItems.Add(Item_Inst);
	}

	slot_stages = PlayerSave_Inst->slot_stages;
	*/
}

void APlayerChar::Equip_Item(int item_index, int slot_index)
{
	SlotItems[slot_index] = InventoryItems[item_index];
	InventoryItems[item_index]->Apply_Effect();
	Remove_Item(item_index);
}

void APlayerChar::UnEquip_Item(int slot_index)
{
	Add_Item(SlotItems[slot_index]);
	SlotItems[slot_index]->Reverse_Effect();
	SlotItems[slot_index] = nullptr;
}

void APlayerChar::Equip_Swap_Item(int item_index, int slot_index)
{
	UItem* Item_Local = SlotItems[slot_index];
	SlotItems[slot_index]->Reverse_Effect();

	Equip_Item(item_index, slot_index);
	Add_Item(Item_Local);
}

void APlayerChar::Remove_Item(int item_index)
{
	if (InventoryCounts[item_index] > 1)
	{
		InventoryCounts[item_index] -= 1;
		Remove_Child_Item(item_index, InventoryCounts[item_index]);
	}
	else
	{
		InventoryItems.RemoveAt(item_index);
		InventoryCounts.RemoveAt(item_index);
		Remove_Child_Item(item_index, 0);
	}
}

void APlayerChar::Add_Item(UItem* Item)
{
	int target_index = 0;

	bool bFoundMatch = false;

	int fixed_size = InventoryItems.Num();

	if (fixed_size != 0)
	{
		for (int i = 0; i < fixed_size; i++)
		{
			if (InventoryItems[i]->Item_Tier == Item->Item_Tier && !bFoundMatch)
			{
				target_index = i;
				bFoundMatch = true;
			}

			if (Item->Item_Name.Equals(InventoryItems[i]->Item_Name))
			{
				InventoryCounts[i] += 1;
				int target_count = InventoryCounts[i];

				for (int j = i; j > target_index; j--)
				{
					InventoryItems[j] = InventoryItems[j - 1];
					InventoryCounts[j] = InventoryCounts[j - 1];
				}

				InventoryItems[target_index] = Item;
				InventoryCounts[target_index] =  target_count;

				Swap_Child_Item(i, target_index, target_count);
				break;
			}
			else if (Item->Item_Tier > InventoryItems[i]->Item_Tier)
			{
				if (bFoundMatch)
				{
					InventoryItems.Insert(Item, target_index);
					InventoryCounts.Insert(1, target_index);
					Add_Child_Item(Item, target_index);
				}
				else
				{
					InventoryItems.Insert(Item, i);
					InventoryCounts.Insert(1, i);
					Add_Child_Item(Item, i);
				}
				break;
			}
			else if (i == (fixed_size - 1))
			{
				if (bFoundMatch)
				{
					InventoryItems.Insert(Item, target_index);
					InventoryCounts.Insert(1, target_index);
					Add_Child_Item(Item, target_index);
				}
				else
				{
					InventoryItems.Add(Item);
					InventoryCounts.Add(1);
					Add_Child_Item(Item, i + 1);
				}
			}
		}
	}
	else
	{
		InventoryItems.Add(Item);
		InventoryCounts.Add(1);
		Add_Child_Item(Item, 0);
	}
}

void APlayerChar::Save()
{
	UPlayerSave* PlayerSave_Inst = Cast<UPlayerSave>(UGameplayStatics::CreateSaveGameObject(UPlayerSave::StaticClass()));

	for (auto InventoryItem : InventoryItems)
	{
		PlayerSave_Inst->ItemClass.Add(InventoryItem->GetClass());
	}

	UGameplayStatics::SaveGameToSlot(PlayerSave_Inst, PlayerSave_Inst->SaveSlotName, PlayerSave_Inst->UserIndex);
}

void APlayerChar::Load()
{
	UPlayerSave* PlayerSave_Inst = Cast<UPlayerSave>(UGameplayStatics::CreateSaveGameObject(UPlayerSave::StaticClass()));
	PlayerSave_Inst = Cast<UPlayerSave>(UGameplayStatics::LoadGameFromSlot(PlayerSave_Inst->SaveSlotName, PlayerSave_Inst->UserIndex));

	for (auto itemClass : PlayerSave_Inst->ItemClass)
	{
		UItem* Item_Inst = NewObject<UItem>(this, itemClass);
		InventoryItems.Add(Item_Inst);
	}
}

void APlayerChar::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CalculateOffset(DeltaSeconds);
	TL_CheckTick(&CounterLine, DeltaSeconds);
}

void APlayerChar::RotLerp(float DeltaTime)
{
	if ((restriction == ERestriction::Restriction_Rot || (bMoveRight != EMoveDirection::Move_None || bMoveForward != EMoveDirection::Move_None)) && !IsDead)
	{
		/*
		FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LerpRotLoc);
		FRotator NewRot = UKismetMathLibrary::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, RotLerpSpeed * CustomTimeDilation);
		SetActorRotation(FQuat(FRotator(0.0f, NewRot.Yaw, 0.0f)));
		*/

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

void APlayerChar::Block_Pressed()
{
	/*
	if (CheckRestriction())
	{
	PlayMontage(BlockMontage, 1.0f, ERestriction::Restriction_Rot, 4.4f);
	IsBlocking = true;
	}
	*/
}

void APlayerChar::Block_Released()
{
	/*
	GetMesh()->GetAnimInstance()->Montage_Stop(0.25f, BlockMontage);
	restriction = ERestriction::Restriction_None;
	IsBlocking = false;
	*/
}

void APlayerChar::ResetMontage_Add()
{
	Super::ResetMontage_Add();

	DidAttack = false;
}

void APlayerChar::ApplyFire()
{
	ChangeElemental(EDamageType::Damage_Fire);
	UpdateIcon(1);
}

void APlayerChar::ApplyIce()
{
	ChangeElemental(EDamageType::Damage_Freeze);
	UpdateIcon(2);
}

void APlayerChar::ApplyPoison()
{
	ChangeElemental(EDamageType::Damage_Poison);
	UpdateIcon(3);
}

void APlayerChar::ApplyLightning()
{
	ChangeElemental(EDamageType::Damage_Lightning);
	UpdateIcon(4);
}

void APlayerChar::ApplyNone()
{
	ChangeElemental(EDamageType::Damage_None);
	UpdateIcon(0);
}

void APlayerChar::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && CheckRestriction())
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
		
		if (Value == 1.0f)
		{
			bMoveForward = EMoveDirection::Move_Backward;
		}
		else
		{
			bMoveForward = EMoveDirection::Move_Forward;
		}

	}
	else
	{
		bMoveForward = EMoveDirection::Move_None;
	}
}

void APlayerChar::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && CheckRestriction())
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		if (Value == 1.0f)
		{
			bMoveRight = EMoveDirection::Move_Right;
		}
		else
		{
			bMoveRight = EMoveDirection::Move_Left;
		}
	}
	else
	{
		bMoveRight = EMoveDirection::Move_None;
	}
}

void APlayerChar::CalculateOffset(float DeltaSeconds)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	FVector2D MousePos;
	bool ValidPos = PC->GetMousePosition(MousePos.X, MousePos.Y);
	
	if (ValidPos)
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		ViewportSize /= 2.0f;

		float dist = 175.0f * FMath::Clamp((MousePos - ViewportSize).Size() / ViewportSize.Y, 0.0f, 1.0f);

		FVector2D ViewportOffset;
		ViewportOffset = (MousePos - ViewportSize).GetSafeNormal().GetRotated(GetCameraBoom()->GetComponentRotation().Yaw + 90.0f);
		ViewportOffset *= dist;

		TargetOffset = FVector(ViewportOffset.X, ViewportOffset.Y, TargetZoom);

		FVector NewOffset = UKismetMathLibrary::VInterpTo(CameraBoom->TargetOffset, TargetOffset, DeltaSeconds, 3.7f);
		CameraBoom->TargetOffset = NewOffset;
	}
}

void APlayerChar::Dodge()
{
	if (CheckRestriction())
	{
		IsInvincible = true;

		const FRotator Rotation = GetCameraBoom()->GetComponentRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		FRotator NewRot = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y).Rotation();

		Play_SoundCue(SoundCue_Dodge, true);

		if (bMoveForward == EMoveDirection::Move_Forward && bMoveRight == EMoveDirection::Move_None)
		{
			NewRot = FRotator(0.0f, -90.0f + Dodge_YawOffset, 0.0f);
		}
		else if (bMoveForward == EMoveDirection::Move_Forward && bMoveRight == EMoveDirection::Move_Left)
		{
			NewRot = FRotator(0.0f, -135.0f + Dodge_YawOffset, 0.0f);
		}
		else if (bMoveForward == EMoveDirection::Move_Forward && bMoveRight == EMoveDirection::Move_Right)
		{
			NewRot = FRotator(0.0f, -45.0f + Dodge_YawOffset, 0.0f);
		}
		else if (bMoveForward == EMoveDirection::Move_Backward && bMoveRight == EMoveDirection::Move_None)
		{
			NewRot = FRotator(0.0f, 90.0f + Dodge_YawOffset, 0.0f);
		}
		else if (bMoveForward == EMoveDirection::Move_Backward && bMoveRight == EMoveDirection::Move_Left)
		{
			NewRot = FRotator(0.0f, 135.0f + Dodge_YawOffset, 0.0f);
		}
		else if (bMoveForward == EMoveDirection::Move_Backward && bMoveRight == EMoveDirection::Move_Right)
		{
			NewRot = FRotator(0.0f, 45.0f + Dodge_YawOffset, 0.0f);
		}
		else if (bMoveForward == EMoveDirection::Move_None && bMoveRight == EMoveDirection::Move_Right)
		{
			NewRot = FRotator(0.0f, 0.0f + Dodge_YawOffset, 0.0f);
		}
		else if (bMoveForward == EMoveDirection::Move_None && bMoveRight == EMoveDirection::Move_Left)
		{
			NewRot = FRotator(0.0f, 180.0f + Dodge_YawOffset, 0.0f);
		}
		else
		{
			NewRot = FRotator(0.0f, NewRot.Yaw + 180.0f + Dodge_YawOffset, 0.0f);
		}

		float DeltaYaw = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), NewRot).Yaw;
		float DeltaABS = UKismetMathLibrary::Abs(DeltaYaw);

		
		if (DeltaABS <= 60.0f)
		{
			PlayMontage(DodgeFrontMontage, 1.0f, ERestriction::Restriction_Fixed_Rot, 7.7f);
		}
		else if (DeltaABS > 60.0f && DeltaABS < 120.0f)
		{
			if (DeltaYaw >= 0.0f)
			{
				NewRot = NewRot + FRotator(0.0f, 90.0f, 0.0f);
				PlayMontage(DodgeLeftMontage, 1.0f, ERestriction::Restriction_Fixed_Rot, 7.7f);
			}
			else
			{
				NewRot = NewRot + FRotator(0.0f, -90.0f, 0.0f);
				PlayMontage(DodgeRightMontage, 1.0f, ERestriction::Restriction_Fixed_Rot, 7.7f);
			}
		}
		else if (DeltaABS >= 120.0f)
		{
			NewRot = NewRot - FRotator(0.0f, 180.0f, 0.0f);
			PlayMontage(DodgeBackMontage, 1.0f, ERestriction::Restriction_Fixed_Rot, 7.7f);
		}

		FixedRot = NewRot;
	}
}

void APlayerChar::CounterFunction(float Value)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), Value);
}

void APlayerChar::ResetCounter()
{
	CounterLine.Reverse();
	TargetZoom = 0.0f;
}

void APlayerChar::UpdateIcon_Implementation(int Power_Type)
{

}

void APlayerChar::AddtoWidget_Implementation(UItem* item)
{

}

void APlayerChar::Die_Add()
{
	TArray<UClass*> Subclasses;

	for (TObjectIterator<UClass> It; It; ++It)
	{
		if (It->IsChildOf(UItem::StaticClass()) && !It->HasAnyClassFlags(CLASS_Abstract))
		{
			Subclasses.Add(*It);
		}
	}

	int rand_class = 0;

	while (true)
	{
		rand_class = UKismetMathLibrary::RandomIntegerInRange(0, Subclasses.Num() - 1);

		if (Subclasses[rand_class] != UItem::StaticClass())
		{
			break;
		}
	}

	UItem* Item_Inst = NewObject<UItem>(this, Subclasses[rand_class]);
}

void APlayerChar::LoadInventory()
{
	if (!IsInventoryActive)
	{
		ChangeWidget(InventoryWidget, true);
		IsInventoryActive = true;
	}
	else
	{
		ChangeWidget(PlayerWidget, true);
		IsInventoryActive = false;
	}
}

void APlayerChar::ChangeWidget(UUserWidget* NewWidget, bool remove)
{
	if (remove && CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
	}

	CurrentWidget = NewWidget;
	CurrentWidget->AddToViewport();
}

void APlayerChar::Widget_Load_Implementation()
{

}

void APlayerChar::Inventory_Items_Load_Implementation()
{

}

void APlayerChar::Add_Child_Item_Implementation(UItem* TargetItem, int target_index)
{

}

void APlayerChar::Swap_Child_Item_Implementation(int first_index, int second_index, int count)
{

}

void APlayerChar::Remove_Child_Item_Implementation(int item_index, int count)
{

}