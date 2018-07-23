// Fill out your copyright notice in the Description page of Project Settings.

#include "MysticCreature.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UMysticCreature::UMysticCreature()
{
	Item_Name = "Spirit of Celestial Life";
	Item_Description = "+1.0 Will Power Refresh Rate \n+10 Will Power";
	Item_Description_Penalty = "-25.0 Health";
	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_blue_37.Tex_blue_37'"));
	Item_Icon = IconObj.Object;
}

void UMysticCreature::Apply_Effect()
{
	MyOwner->WillPower_Rate += 1.0f;

	MyOwner->Update_WillPower_Ratio(10.0f);

	MyOwner->Update_Health_Ratio(-25.0f);
}

void UMysticCreature::Reverse_Effect()
{
	MyOwner->WillPower_Rate -= 1.0f;

	MyOwner->Update_WillPower_Ratio(-10.0f);

	MyOwner->Update_Health_Ratio(25.0f);
}