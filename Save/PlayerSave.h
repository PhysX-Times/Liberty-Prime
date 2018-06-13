// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSave.generated.h"

class UItem;
class APlayerChar;

UCLASS()
class LIBERTYPRIME_API UPlayerSave : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPlayerSave();

	UPROPERTY()
	APlayerChar* MyOwner;

	UPROPERTY()
	int PlayerLevel;
	UPROPERTY()
	int UnusedPerk;
	UPROPERTY()
	float PlayerDMG;
	UPROPERTY()
	float PlayerHealth;
	UPROPERTY()
	float FreezeAmount;
	UPROPERTY()
	float FreezeDuration;
	UPROPERTY()
	float FreezeResist_Duration;
	UPROPERTY()
	float FreezeResist_Frostbite;
	UPROPERTY()
	float FireDMG;
	UPROPERTY()
	float FireResist_Duration;
	UPROPERTY()
	float FireResist_DMG;
	UPROPERTY()
	int FireTick;
	UPROPERTY()
	float PoisonDMG;
	UPROPERTY()
	float PoisonResist_Duration;
	UPROPERTY()
	float PoisonResist_DMG;
	UPROPERTY()
	int PoisonTick;
	UPROPERTY()
	float PhysicalResist_DMG;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	TArray<UClass*> ItemClass;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
	TArray<int32> slot_stages;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;
};
