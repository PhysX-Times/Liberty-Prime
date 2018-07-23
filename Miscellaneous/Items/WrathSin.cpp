// Fill out your copyright notice in the Description page of Project Settings.

#include "WrathSin.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UWrathSin::UWrathSin()
{
	Item_Name = "Sin of Wrath";
	Item_Description = "+10.0 Fire Damage \n+8.0 Fire Resistance";
	Item_Description_Penalty = "-15.0 Health \n-15.0 Will Power";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_red_10.Tex_red_10'"));
	Item_Icon = IconObj.Object;
}

void UWrathSin::Apply_Effect()
{
	MyOwner->FireDMG += 10.0f;
	MyOwner->FireDMG_Item += 10.0f;

	MyOwner->FireResist_DMG += 8.0f;
	MyOwner->FireResist_Item += 8.0f;

	MyOwner->Update_Health_Ratio(-15.0f);

	MyOwner->Update_WillPower_Ratio(-15.0f);
}

void UWrathSin::Reverse_Effect()
{
	MyOwner->FireDMG -= 10.0f;
	MyOwner->FireDMG_Item -= 10.0f;

	MyOwner->FireResist_DMG -= 8.0f;
	MyOwner->FireResist_Item -= 8.0f;

	MyOwner->Update_Health_Ratio(15.0f);

	MyOwner->Update_WillPower_Ratio(15.0f);
}