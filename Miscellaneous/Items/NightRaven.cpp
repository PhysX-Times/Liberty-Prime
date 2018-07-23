// Fill out your copyright notice in the Description page of Project Settings.

#include "NightRaven.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UNightRaven::UNightRaven()
{
	Item_Name = "Night Raven";
	Item_Description = "+5% Lifesteal \n+5% Glory Gain \n+50.0 Health \n+25.0 Will Power \n+0.5 Poison Resistance \n+0.05 Frostbite Resistance";
	Item_Description_Penalty = "-10.0 Fire Resistance \n-20.0 Lightning Resistance";
	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_violet_18.Tex_violet_18'"));
	Item_Icon = IconObj.Object;
}

void UNightRaven::Apply_Effect()
{
	MyOwner->Lifesteal_Rate += 5.0f;

	MyOwner->exp_multiplier += 0.5f;

	MyOwner->Update_Health_Ratio(50.0f);

	MyOwner->Update_WillPower_Ratio(25.0f);

	MyOwner->PoisonResist_DMG += 0.5f;
	MyOwner->PoisonResist_Item += 0.5f;

	MyOwner->FreezeResist_Frostbite += 0.05f;
	MyOwner->FrostbiteResist_Item += 0.05f;

	MyOwner->FireResist_DMG -= 10.0f;
	MyOwner->FireResist_Item -= 10.0f;

	MyOwner->LightningResist_DMG -= 20.0f;
	MyOwner->LightningResist_Item -= 20.0f;
}

void UNightRaven::Reverse_Effect()
{
	MyOwner->Lifesteal_Rate -= 5.0f;

	MyOwner->exp_multiplier -= 0.5f;

	MyOwner->Update_Health_Ratio(-50.0f);

	MyOwner->Update_WillPower_Ratio(-25.0f);

	MyOwner->PoisonResist_DMG -= 0.5f;
	MyOwner->PoisonResist_Item -= 0.5f;

	MyOwner->FreezeResist_Frostbite -= 0.05f;
	MyOwner->FrostbiteResist_Item -= 0.05f;

	MyOwner->FireResist_DMG += 10.0f;
	MyOwner->FireResist_Item += 10.0f;

	MyOwner->LightningResist_DMG += 20.0f;
	MyOwner->LightningResist_Item += 20.0f;
}