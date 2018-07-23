// Fill out your copyright notice in the Description page of Project Settings.

#include "Carnage.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UCarnage::UCarnage()
{
	Item_Name = "Blade of Carnage";
	Item_Description = "+10.0 Physical Damage";
	Item_Description_Penalty = "-20% Glory Gain";
	Item_Tier = 1;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset1/45.45'"));
	Item_Icon = IconObj.Object;
}

void UCarnage::Apply_Effect()
{
	MyOwner->DMG += 10.0f;
	MyOwner->DMG_Item += 10.0f;

	MyOwner->exp_multiplier -= 0.2f;
}

void UCarnage::Reverse_Effect()
{
	MyOwner->DMG -= 10.0f;
	MyOwner->DMG_Item -= 10.0f;

	MyOwner->exp_multiplier += 0.2f;
}