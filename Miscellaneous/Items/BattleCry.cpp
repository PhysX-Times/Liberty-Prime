// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleCry.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UBattleCry::UBattleCry()
{
	Item_Name = "Battle Cry";
	Item_Description = "+30.0 Will Power \n+5.0 Physical Damage";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_yellow_17.Tex_yellow_17'"));
	Item_Icon = IconObj.Object;
}

void UBattleCry::Apply_Effect()
{
	MyOwner->Update_WillPower_Ratio(30.0f);

	MyOwner->DMG += 5.0f;
	MyOwner->DMG_Item += 5.0f;
}

void UBattleCry::Reverse_Effect()
{
	MyOwner->Update_WillPower_Ratio(-30.0f);

	MyOwner->DMG -= 5.0f;
	MyOwner->DMG_Item -= 5.0f;
}