// Fill out your copyright notice in the Description page of Project Settings.

#include "VirtueJustice.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UVirtueJustice::UVirtueJustice()
{
	Item_Name = "Virtue of Justice";
	Item_Description = "+10% Glory Gain \n+25.0 Will Power";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_yellow_13.Tex_yellow_13'"));
	Item_Icon = IconObj.Object;
}

void UVirtueJustice::Apply_Effect()
{
	MyOwner->exp_multiplier += 0.1f;

	MyOwner->Update_WillPower_Ratio(25.0f);
}

void UVirtueJustice::Reverse_Effect()
{
	MyOwner->exp_multiplier -= 0.1f;

	MyOwner->Update_WillPower_Ratio(-25.0f);
}