// Fill out your copyright notice in the Description page of Project Settings.

#include "DemonHeart.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UDemonHeart::UDemonHeart()
{
	Item_Name = "Heart of Doom";
	Item_Description = "+100.0 Health";
	Item_Description_Penalty = "-30.0 Will Power";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_red_33.Tex_red_33'"));
	Item_Icon = IconObj.Object;
}

void UDemonHeart::Apply_Effect()
{
	MyOwner->Update_Health_Ratio(100.0f);

	MyOwner->Update_WillPower_Ratio(-30.0f);
}

void UDemonHeart::Reverse_Effect()
{
	MyOwner->Update_Health_Ratio(-100.0f);

	MyOwner->Update_WillPower_Ratio(30.0f);
}