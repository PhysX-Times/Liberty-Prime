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
#include "LibertyPrimePlayerController.h"

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

	WillPower_Rate = 1.0f;
	Lifesteal_Rate = 0.0f;

	bCanUseDash = true;
	bCanUseSpin = true;
	bCanUseEnergy = true;

	bPoisonImmunity = false;
	bShield = true;

	shield_block_plus = 0.0f;
}

void APlayerChar::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Attack", IE_Pressed, this, &APlayerChar::AttackFunction);
	InputComponent->BindAction("Block", IE_Pressed, this, &APlayerChar::Block_Pressed);
	InputComponent->BindAction("Block", IE_Released, this, &APlayerChar::Block_Released);
	InputComponent->BindAction("Skill-1", IE_Pressed, this, &APlayerChar::DashFunction);
	InputComponent->BindAction("Skill-2", IE_Pressed, this, &APlayerChar::SpinFunction);
	InputComponent->BindAction("Skill-3", IE_Pressed, this, &APlayerChar::EnergyFunction);
	InputComponent->BindAction("Health", IE_Pressed, this, &APlayerChar::Use_Heal_Potion);
	InputComponent->BindAction("WillPower", IE_Pressed, this, &APlayerChar::Use_WillPower_Potion);

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

void APlayerChar::Use_Heal_Potion()
{
	if (potion_heal_count > 0 && Health < MaxHealth && !IsDead)
	{
		Health = FMath::Clamp(Health + potion_heal_amount, 0.0f, MaxHealth);
		UpdateHealth_PB();
		potion_heal_count -= 1;
		Update_PotionCount_Health();

		Poison_Clear();

		UGameplayStatics::SpawnEmitterAttached(Health_PS, GetMesh());
		Play_SoundCue_2D(SoundCue_HealthPotion);
	}
}

void APlayerChar::Use_WillPower_Potion()
{
	if (potion_willpower_count > 0 && WillPower < MaxWillPower && !IsDead)
	{
		WillPower = FMath::Clamp(WillPower + potion_willpower_amount, 0.0f, MaxWillPower);
		Update_WillPower();
		potion_willpower_count -= 1;
		Update_PotionCount_WillPower();

		UGameplayStatics::SpawnEmitterAttached(WillPower_PS, GetMesh());
		Play_SoundCue_2D(SoundCue_WillPowerPotion);
	}
}

void APlayerChar::DashFunction()
{
	if (bCanUseDash && CheckRestriction())
	{
		if (Check_Use_WillPower(10.0f))
		{
			Dash_Call();
			DashAttack();
			bCanUseDash = false;
			GetWorldTimerManager().SetTimer(DashTimer, this, &APlayerChar::Reset_Dash, 4.0f, false);
		}
	}
}

void APlayerChar::SpinFunction()
{
	if (bCanUseSpin && CheckRestriction())
	{
		if (Check_Use_WillPower(20.0f))
		{
			Spin_Call();
			SpinAttack();
			bCanUseSpin = false;
			GetWorldTimerManager().SetTimer(SpinTimer, this, &APlayerChar::Reset_Spin, 7.0f, false);
		}
	}
}

void APlayerChar::EnergyFunction()
{
	if (bCanUseEnergy && CheckRestriction())
	{
		if (Check_Use_WillPower(15.0f))
		{
			Energy_Call();
			EnergyAttack();
			bCanUseEnergy = false;
			GetWorldTimerManager().SetTimer(EnergyTimer, this, &APlayerChar::Reset_Energy, 8.5f, false);
		}
	}
}

void APlayerChar::Reset_Dash()
{
	bCanUseDash = true;
}

void APlayerChar::Reset_Spin()
{
	bCanUseSpin = true;
}

void APlayerChar::Reset_Energy()
{
	bCanUseEnergy = true;
}

bool APlayerChar::Check_Use_WillPower(float amount)
{
	if (WillPower > amount)
	{
		WillPower -= amount;
		Update_WillPower();
		GetWorldTimerManager().ClearTimer(WP_ReshreshTimer);
		GetWorldTimerManager().SetTimer(WP_ReshreshTimer, this, &APlayerChar::WillPower_Refresh, 2.5f, false);
		return true;
	}
	else
	{
		return false;
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

void APlayerChar::WillPower_Refresh()
{
	if (WillPower < MaxWillPower)
	{
		WillPower = FMath::Clamp(WillPower + (WillPower_Rate / 10.0f), 0.0f, MaxWillPower);
		Update_WillPower();
		GetWorldTimerManager().SetTimer(WP_ReshreshTimer, this, &APlayerChar::WillPower_Refresh, 0.1f, false);
	}
}

void APlayerChar::Lifesteal(float value)
{
	UGameplayStatics::SpawnEmitterAttached(Lifesteal_PS, GetMesh());

	if (Health < MaxHealth)
	{
		Health = FMath::Clamp(Health + value, 0.0f, MaxHealth);
		UpdateHealth_PB();
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

void APlayerChar::Damager(FDamageData DamageData, FVector PSLoc)
{
	if (!IsDead)
	{
		float dmg_multiplied = DamageData.Applier->DMG * DamageData.AttackData->DMGMultiplier;
		float TargetDMG = Calculate_Effect(dmg_multiplied, PhysicalResist_DMG);

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

			DamageData.Applier->Damager_Simple(LightningDMG_Local / 100.0f * 15.0f, LightningColor);
			TargetDMG += LightningDMG_Local;

			if (PlayerController)
			{
				PlayerController->Add_Indicator(LightningDMG_Local, LightningColor, PSLoc + FVector(0.0f, 0.0f, 47.5f));
			}
		}

		if (bShield)
		{
			FVector Offset = (GetActorLocation() - DamageData.Source->GetActorLocation()).GetSafeNormal();

			float DotProduct = FVector::DotProduct(GetActorForwardVector(), Offset);

			if (DotProduct >= 0.0f)
			{
				TargetDMG = TargetDMG * (0.7f - shield_block_plus);
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Frontal Shield");
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

void APlayerChar::ApplyElemental(ALibertyPrimeCharacter* Applier, EDamageType DMGType)
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
		if (!bPoisonImmunity)
		{
			Poison(Applier->PoisonTick, Calculate_Effect(Applier->PoisonDMG, PoisonResist_DMG));
			break;
		}
	}
	default:
		break;
	}
}

void APlayerChar::Update_Health_Ratio(float value)
{
	float ratio = MaxHealth / Health;

	MaxHealth += value;
	MaxHealth_Item += value;

	Health = MaxHealth / ratio;
}

void APlayerChar::Update_WillPower_Ratio(float value)
{
	float ratio = MaxWillPower / WillPower;

	MaxWillPower += value;
	MaxWillPower_Item += value;

	WillPower = MaxWillPower / ratio;
}

void APlayerChar::Update_WillPower_Implementation()
{

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

void APlayerChar::Dash_Call_Implementation()
{

}

void APlayerChar::Spin_Call_Implementation()
{

}

void APlayerChar::Energy_Call_Implementation()
{

}

void APlayerChar::Update_PotionCount_Health_Implementation()
{

}

void APlayerChar::Update_PotionCount_WillPower_Implementation()
{

}