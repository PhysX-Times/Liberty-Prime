// Fill out your copyright notice in the Description page of Project Settings.

#include "CursePenance.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UCursePenance::UCursePenance()
{
	Item_Name = "Curse of Penance";
	Item_Description = "+5% Glory Gain \n+50.0 Will Power \n+10.0 Lightning Damage";
	Item_Description_Penalty = "-2.0 Physical Damage \n-0.2 Poison Resistance";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_violet_20.Tex_violet_20'"));
	Item_Icon = IconObj.Object;
}

void UCursePenance::Apply_Effect()
{
	MyOwner->exp_multiplier += 0.5f;

	MyOwner->Update_WillPower_Ratio(50.0f);

	MyOwner->LightningDMG += 10.0f;
	MyOwner->LightningDMG_Item += 10.0f;

	MyOwner->DMG -= 2.0f;
	MyOwner->DMG_Item -= 2.0f;

	MyOwner->PoisonResist_DMG -= 0.2f;
	MyOwner->PoisonResist_Item -= 0.2f;
}

void UCursePenance::Reverse_Effect()
{
	MyOwner->exp_multiplier -= 0.5f;

	MyOwner->Update_WillPower_Ratio(-50.0f);

	MyOwner->LightningDMG -= 10.0f;
	MyOwner->LightningDMG_Item -= 10.0f;

	MyOwner->DMG += 2.0f;
	MyOwner->DMG_Item += 2.0f;

	MyOwner->PoisonResist_DMG += 0.2f;
	MyOwner->PoisonResist_Item += 0.2f;
}