// Fill out your copyright notice in the Description page of Project Settings.

#include "VirtueCourage.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UVirtueCourage::UVirtueCourage()
{
	Item_Name = "Virtue of Courage";
	Item_Description = "+35.0 Health \n+15.0 Will Power \n+2.0 Physical Damage";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_yellow_14.Tex_yellow_14'"));
	Item_Icon = IconObj.Object;
}

void UVirtueCourage::Apply_Effect()
{
	MyOwner->Update_Health_Ratio(35.0f);

	MyOwner->Update_WillPower_Ratio(15.0f);

	MyOwner->DMG += 2.0f;
	MyOwner->DMG_Item += 2.0f;
}

void UVirtueCourage::Reverse_Effect()
{
	MyOwner->Update_Health_Ratio(-35.0f);

	MyOwner->Update_WillPower_Ratio(-15.0f);

	MyOwner->DMG -= 2.0f;
	MyOwner->DMG_Item -= 2.0f;
}