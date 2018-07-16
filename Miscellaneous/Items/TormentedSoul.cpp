// Fill out your copyright notice in the Description page of Project Settings.

#include "TormentedSoul.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UTormentedSoul::UTormentedSoul()
{
	Item_Name = "Tormented Souls";
	Item_Description = "+5.0 Fire Damage \n+10.0 Fire Resistance";
	Item_Description_Penalty = "-20.0 Will Power";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_yellow_19.Tex_yellow_19'"));
	Item_Icon = IconObj.Object;
}

void UTormentedSoul::Apply_Effect()
{
	MyOwner->FireDMG += 5.0f;
	MyOwner->FireDMG_Item += 5.0f;

	MyOwner->FireResist_DMG += 10.0f;
	MyOwner->FireResist_Item += 10.0f;

	MyOwner->MaxWillPower -= 20.0f;
	MyOwner->MaxWillPower_Item -= 20.0f;
	MyOwner->WillPower -= 20.0f;
}

void UTormentedSoul::Reverse_Effect()
{
	MyOwner->FireDMG -= 5.0f;
	MyOwner->FireDMG_Item -= 5.0f;

	MyOwner->FireResist_DMG -= 10.0f;
	MyOwner->FireResist_Item -= 10.0f;

	MyOwner->MaxWillPower += 20.0f;
	MyOwner->MaxWillPower_Item += 20.0f;
	MyOwner->WillPower += 20.0f;
}