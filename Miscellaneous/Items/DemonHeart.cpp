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
	MyOwner->MaxHealth += 100.0f;
	MyOwner->MaxHealth_Item += 100.0f;
	MyOwner->Health += 100.0f;

	MyOwner->MaxWillPower -= 30.0f;
	MyOwner->MaxWillPower_Item -= 30.0f;
	MyOwner->WillPower -= 30.0f;
}

void UDemonHeart::Reverse_Effect()
{
	MyOwner->MaxHealth -= 100.0f;
	MyOwner->MaxHealth_Item -= 100.0f;
	MyOwner->Health -= 100.0f;

	MyOwner->MaxWillPower += 30.0f;
	MyOwner->MaxWillPower_Item += 30.0f;
	MyOwner->WillPower += 30.0f;
}