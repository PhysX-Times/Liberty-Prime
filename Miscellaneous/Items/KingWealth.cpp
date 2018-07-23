// Fill out your copyright notice in the Description page of Project Settings.

#include "KingWealth.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UKingWealth::UKingWealth()
{
	Item_Name = "Emperor's Wealth";
	Item_Description = "+50% Glory Gain";
	Item_Description_Penalty = "-2.0 Physical Resistance \n-4.0 Fire Resistance \n-0.2 Poison Resistance \n-8.0 Lightning Resistance";
	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset1/94.94'"));
	Item_Icon = IconObj.Object;
}

void UKingWealth::Apply_Effect()
{
	MyOwner->exp_multiplier += 0.5f;

	MyOwner->PhysicalResist_DMG -= 2.0f;
	MyOwner->PhysicalResist_Item -= 2.0f;

	MyOwner->FireResist_DMG -= 4.0f;
	MyOwner->FireResist_Item -= 4.0f;

	MyOwner->PoisonResist_DMG -= 0.2f;
	MyOwner->PoisonResist_Item -= 0.2f;

	MyOwner->LightningResist_DMG -= 8.0f;
	MyOwner->LightningResist_Item -= 8.0f;
}

void UKingWealth::Reverse_Effect()
{
	MyOwner->exp_multiplier -= 0.5f;

	MyOwner->PhysicalResist_DMG += 2.0f;
	MyOwner->PhysicalResist_Item += 2.0f;

	MyOwner->FireResist_DMG += 4.0f;
	MyOwner->FireResist_Item += 4.0f;

	MyOwner->PoisonResist_DMG += 0.2f;
	MyOwner->PoisonResist_Item += 0.2f;

	MyOwner->LightningResist_DMG += 8.0f;
	MyOwner->LightningResist_Item += 8.0f;
}