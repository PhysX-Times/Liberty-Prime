// Fill out your copyright notice in the Description page of Project Settings.

#include "EnvySin.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UEnvySin::UEnvySin()
{
	Item_Name = "Sin of Envy";
	Item_Description = "+5% Lifesteal \n+30 Will Power \n+4.0 Physical Damage \n+4.0 Fire Damage \n+4.0 Fire Resistance";
	Item_Description_Penalty = "-20.0% Glory Gain";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_yellow_18.Tex_yellow_18'"));
	Item_Icon = IconObj.Object;
}

void UEnvySin::Apply_Effect()
{
	MyOwner->Lifesteal_Rate += 5.0f;
	
	MyOwner->Update_WillPower_Ratio(30.0f);

	MyOwner->DMG += 4.0f;
	MyOwner->DMG_Item += 4.0f;

	MyOwner->FireDMG += 4.0f;
	MyOwner->FireDMG_Item += 4.0f;

	MyOwner->FireResist_DMG += 4.0f;
	MyOwner->FireResist_Item += 4.0f;

	MyOwner->exp_multiplier -= 0.2f;
}

void UEnvySin::Reverse_Effect()
{
	MyOwner->Lifesteal_Rate -= 5.0f;

	MyOwner->Update_WillPower_Ratio(-30.0f);

	MyOwner->DMG -= 4.0f;
	MyOwner->DMG_Item -= 4.0f;

	MyOwner->FireDMG -= 4.0f;
	MyOwner->FireDMG_Item -= 4.0f;

	MyOwner->FireResist_DMG -= 4.0f;
	MyOwner->FireResist_Item -= 4.0f;

	MyOwner->exp_multiplier += 0.2f;
}