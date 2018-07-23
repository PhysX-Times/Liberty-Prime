// Fill out your copyright notice in the Description page of Project Settings.

#include "JackTrade.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UJackTrade::UJackTrade()
{
	Item_Name = "Jack of All Trades";
	Item_Description = "+2.0 Physical Damage \n+4.0 Fire Damage \n+0.2 Poison Damage \n+8.0 Lightning Damage";
	Item_Description_Penalty = "-15% Glory Gain";
	Item_Tier = 1;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_gray_5.Tex_gray_5'"));
	Item_Icon = IconObj.Object;
}

void UJackTrade::Apply_Effect()
{
	MyOwner->DMG += 2.0f;
	MyOwner->DMG_Item += 2.0f;

	MyOwner->FireDMG += 4.0f;
	MyOwner->FireDMG_Item += 4.0f;

	MyOwner->PoisonDMG += 0.2f;
	MyOwner->PoisonDMG_Item += 0.2f;

	MyOwner->LightningDMG += 8.0f;
	MyOwner->LightningDMG_Item += 8.0f;

	MyOwner->exp_multiplier -= 0.15f;
}

void UJackTrade::Reverse_Effect()
{
	MyOwner->DMG -= 2.0f;
	MyOwner->DMG_Item -= 2.0f;

	MyOwner->FireDMG -= 4.0f;
	MyOwner->FireDMG_Item -= 4.0f;

	MyOwner->PoisonDMG -= 0.2f;
	MyOwner->PoisonDMG_Item -= 0.2f;

	MyOwner->LightningDMG -= 8.0f;
	MyOwner->LightningDMG_Item -= 8.0f;

	MyOwner->exp_multiplier += 0.15f;
}