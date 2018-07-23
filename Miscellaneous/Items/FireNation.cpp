// Fill out your copyright notice in the Description page of Project Settings.

#include "FireNation.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UFireNation::UFireNation()
{
	Item_Name = "The Fire Nation Army";
	Item_Description = "+25.0 Health \n+5.0 Physical Damage \n+10.0 Fire Damage";
	Item_Description_Penalty = "-0.05 Frostbite Resistance";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_red_34.Tex_red_34'"));
	Item_Icon = IconObj.Object;
}

void UFireNation::Apply_Effect()
{
	MyOwner->Update_Health_Ratio(25.0f);

	MyOwner->DMG += 5.0f;
	MyOwner->DMG_Item += 5.0f;

	MyOwner->FireDMG += 10.0f;
	MyOwner->FireDMG_Item += 10.0f;

	MyOwner->FreezeResist_Frostbite -= 0.05f;
	MyOwner->FrostbiteResist_Item -= 0.05f;
}

void UFireNation::Reverse_Effect()
{
	MyOwner->Update_Health_Ratio(-25.0f);

	MyOwner->DMG -= 5.0f;
	MyOwner->DMG_Item -= 5.0f;

	MyOwner->FireDMG -= 10.0f;
	MyOwner->FireDMG_Item -= 10.0f;

	MyOwner->FreezeResist_Frostbite += 0.05f;
	MyOwner->FrostbiteResist_Item += 0.05f;
}