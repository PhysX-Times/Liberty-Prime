// Fill out your copyright notice in the Description page of Project Settings.

#include "LimboTorture.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

ULimboTorture::ULimboTorture()
{
	Item_Name = "Torture of The Limbo";
	Item_Description = "+0.7 Poison Resistance \n+5.0 Physical Resistance";
	Item_Description_Penalty = "-30.0 Will Power";
	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset1/100.100'"));
	Item_Icon = IconObj.Object;
}

void ULimboTorture::Apply_Effect()
{
	MyOwner->PoisonResist_DMG += 0.7f;
	MyOwner->PoisonResist_Item += 0.7f;

	MyOwner->PhysicalResist_DMG += 5.0f;
	MyOwner->PhysicalResist_Item += 5.0f;

	MyOwner->Update_WillPower_Ratio(-30.0f);
}

void ULimboTorture::Reverse_Effect()
{
	MyOwner->PoisonResist_DMG -= 0.7f;
	MyOwner->PoisonResist_Item -= 0.7f;

	MyOwner->PhysicalResist_DMG -= 5.0f;
	MyOwner->PhysicalResist_Item -= 5.0f;

	MyOwner->Update_WillPower_Ratio(30.0f);
}