// Fill out your copyright notice in the Description page of Project Settings.

#include "JusticeAxe.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UJusticeAxe::UJusticeAxe()
{
	Item_Name = "Greataxe of Eternal Justice";
	Item_Description = "+25.0 Health \n+4.0 Physical Damage";
	Item_Description_Penalty = "-10.0 Will Power";
	Item_Tier = 1;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset1/46.46'"));
	Item_Icon = IconObj.Object;
}

void UJusticeAxe::Apply_Effect()
{
	MyOwner->Update_Health_Ratio(25.0f);

	MyOwner->Update_WillPower_Ratio(-10.0f);

	MyOwner->DMG += 4.0f;
	MyOwner->DMG_Item += 4.0f;
}

void UJusticeAxe::Reverse_Effect()
{
	MyOwner->Update_Health_Ratio(-25.0f);

	MyOwner->Update_WillPower_Ratio(10.0f);

	MyOwner->DMG -= 4.0f;
	MyOwner->DMG_Item -= 4.0f;
}