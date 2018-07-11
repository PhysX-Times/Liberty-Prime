// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Knight/SnSKnight.h"
#include "Blueprint/UserWidget.h"
#include "PlayerChar.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EMoveDirection : uint8
{
	Move_Forward 	UMETA(DisplayName = "Move_Forward"),
	Move_Backward 	UMETA(DisplayName = "Move_Backward"),
	Move_Right 	UMETA(DisplayName = "Move_Right"),
	Move_Left 	UMETA(DisplayName = "Move_Left"),
	Move_None	UMETA(DisplayName = "Move_None")
};

class UItem;

UCLASS()
class LIBERTYPRIME_API APlayerChar : public ASnSKnight
{
	GENERATED_BODY()

public:

	APlayerChar();

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		float MaxHealth_Item;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		float MaxWillPower_Item;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		float DMG_Item;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		float PhysicalResist_Item;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		float FireDMG_Item;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		float FireResist_Item;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		float LightningDMG_Item;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		float LightningResist_Item;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		float PoisonDMG_Item;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		float PoisonResist_Item;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		float Frostbite_Item;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		float FrostbiteResist_Item;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
	float credit;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
	TArray<int32> slot_stages;

	float TargetZoom;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	float Dodge_YawOffset;

	bool DidAttack;
	bool IsInventoryActive;

	UPROPERTY()
	UCurveFloat* CounterCurveFloat;

	EMoveDirection bMoveForward;
	EMoveDirection bMoveRight;

	FVector TargetOffset;

	FTimeline CounterLine;

	FTimerHandle IronWillTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG")
	TSubclassOf<UUserWidget> InventoryWidget_Class;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG")
	TSubclassOf<UUserWidget> PlayerWidget_Class;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<class ASword> SwordClass;

	UPROPERTY(BlueprintReadWrite, Category = "UMG")
	UUserWidget* CurrentWidget;
	UPROPERTY(BlueprintReadWrite, Category = "UMG")
	UUserWidget* InventoryWidget;
	UPROPERTY(BlueprintReadWrite, Category = "UMG")
	UUserWidget* PlayerWidget;

	UPROPERTY()
		TArray<UAttackData*> CounterDatas;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
		TArray<int> InventoryCounts;

	
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
		TArray<UItem*> InventoryItems;
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
		TArray<UItem*> SlotItems;

	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* DodgeFrontMontage;
	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* DodgeBackMontage;
	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* DodgeLeftMontage;
	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* DodgeRightMontage;
	UPROPERTY(EditAnywhere, Category = "Montages")
		UAnimMontage* BlockMontage;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Dodge;

	UPROPERTY()
		UAttackData* AttackData_const;

	void CalculateOffset(float DeltaSeconds);

	void AttackFunction();
	void Block_Pressed();
	void Block_Released();
	void ResetCounter();
	void Dodge();

	void AttackCheck() override;
	void AttackSequenceCheck() override;
	void ResetMontage_Add() override;
	void RotLerp(float DeltaTime) override;
	void Die_Add() override;

	void IronWill_Apply();

	void ApplyFire();
	void ApplyIce();
	void ApplyPoison();
	void ApplyLightning();
	void ApplyNone();

	void Save();
	void Load();

	void LoadInventory();

	void ChangeWidget(UUserWidget* NewWidget, bool remove);

	void Add_Item(UItem* Item);
	void Remove_Item(int item_index);

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Equip_Item(int item_index, int slot_index);
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Equip_Swap_Item(int item_index, int slot_index);
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void UnEquip_Item(int slot_index);

	UFUNCTION(BlueprintImplementableEvent, Category = "CppFunctions")
		void Add_Child_Item(UItem* TargetItem, int target_index);
		void Add_Child_Item_Implementation(UItem* TargetItem, int target_index);
	UFUNCTION(BlueprintImplementableEvent, Category = "CppFunctions")
		void Swap_Child_Item(int first_index, int second_index, int count);
		void Swap_Child_Item_Implementation(int first_index, int second_index, int count);
	UFUNCTION(BlueprintImplementableEvent, Category = "CppFunctions")
		void Remove_Child_Item(int item_index, int count);
		void Remove_Child_Item_Implementation(int item_index, int count);

	UFUNCTION()
	void CounterFunction(float Value);

	UFUNCTION(BlueprintNativeEvent, Category = "CppFunctions")
	void UpdateIcon(int Power_Type);

	UFUNCTION(BlueprintNativeEvent, Category = "CppFunctions")
	void AddtoWidget(UItem* item);

	UFUNCTION(BlueprintImplementableEvent, Category = "CppFunctions")
	void Widget_Load();
	void Widget_Load_Implementation();

	UFUNCTION(BlueprintImplementableEvent, Category = "CppFunctions")
	void Inventory_Items_Load();
	void Inventory_Items_Load_Implementation();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UPointLightComponent* AreaLightComp;

	void MoveForward(float Value);
	void MoveRight(float Value);
};
