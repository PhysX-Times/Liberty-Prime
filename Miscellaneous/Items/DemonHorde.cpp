// Fill out your copyright notice in the Description page of Project Settings.

#include "DemonHorde.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UDemonHorde::UDemonHorde()
{
	Item_Name = "Terror of Demon Horde";
	Item_Description = "+30.0 Health \n+30.0 Will Power \n+3.0 Physical Damage";
	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_red_2.Tex_red_2'"));
	Item_Icon = IconObj.Object;
}

void UDemonHorde::Apply_Effect()
{
	MyOwner->Update_Health_Ratio(30.0f);

	MyOwner->Update_WillPower_Ratio(30.0f);

	MyOwner->DMG += 3.0f;
	MyOwner->DMG_Item += 3.0f;
}

void UDemonHorde::Reverse_Effect()
{
	MyOwner->Update_Health_Ratio(-30.0f);

	MyOwner->Update_WillPower_Ratio(-30.0f);

	MyOwner->DMG -= 3.0f;
	MyOwner->DMG_Item -= 3.0f;
}