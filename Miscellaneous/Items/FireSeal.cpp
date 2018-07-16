// Fill out your copyright notice in the Description page of Project Settings.

#include "FireSeal.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UFireSeal::UFireSeal()
{
	Item_Name = "Fire Seal";
	Item_Description = "+10.0 Fire Damage \n+4.0 Fire Resistance";
	Item_Description_Penalty = "-2.0 Physical Damage \n-2.0 Physical Resistance";
	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_ff_09.Tex_ff_09'"));
	Item_Icon = IconObj.Object;
}

void UFireSeal::Apply_Effect()
{
	MyOwner->FireDMG += 10.0f;
	MyOwner->FireDMG_Item += 10.0f;
	MyOwner->FireResist_DMG += 4.0f;
	MyOwner->FireResist_Item += 4.0f;

	MyOwner->DMG -= 2.0f;
	MyOwner->DMG_Item -= 2.0f;
	MyOwner->PhysicalResist_DMG -= 2.0f;
	MyOwner->PhysicalResist_Item -= 2.0f;
}

void UFireSeal::Reverse_Effect()
{
	MyOwner->FireDMG -= 10.0f;
	MyOwner->FireDMG_Item -= 10.0f;
	MyOwner->FireResist_DMG -= 4.0f;
	MyOwner->FireResist_Item -= 4.0f;

	MyOwner->DMG += 2.0f;
	MyOwner->DMG_Item += 2.0f;
	MyOwner->PhysicalResist_DMG += 2.0f;
	MyOwner->PhysicalResist_Item += 2.0f;
}