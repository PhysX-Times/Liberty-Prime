// Fill out your copyright notice in the Description page of Project Settings.

#include "GreedSin.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UGreedSin::UGreedSin()
{
	Item_Name = "Sin of Greed";
	Item_Description = "+30.0% Glory Gain";
	Item_Description_Penalty = "-2.0 Physical Damage \n-4.0 Fire Damage \n-0.2 Poison Damage \n-8.0 Lightning Damage";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_yellow_20.Tex_yellow_20'"));
	Item_Icon = IconObj.Object;
}

void UGreedSin::Apply_Effect()
{
	MyOwner->exp_multiplier += 0.3f;

	MyOwner->DMG -= 2.0f;
	MyOwner->DMG_Item -= 2.0f;

	MyOwner->FireDMG -= 4.0f;
	MyOwner->FireDMG_Item -= 4.0f;

	MyOwner->PoisonDMG -= 0.2f;
	MyOwner->PoisonDMG_Item -= 0.2f;

	MyOwner->LightningDMG -= 8.0f;
	MyOwner->LightningDMG_Item -= 8.0f;
}

void UGreedSin::Reverse_Effect()
{
	MyOwner->exp_multiplier -= 0.3f;

	MyOwner->DMG += 2.0f;
	MyOwner->DMG_Item += 2.0f;

	MyOwner->FireDMG += 4.0f;
	MyOwner->FireDMG_Item += 4.0f;

	MyOwner->PoisonDMG += 0.2f;
	MyOwner->PoisonDMG_Item += 0.2f;

	MyOwner->LightningDMG += 8.0f;
	MyOwner->LightningDMG_Item += 8.0f;
}