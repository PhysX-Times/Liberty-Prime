// Fill out your copyright notice in the Description page of Project Settings.

#include "LifeDevourer.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

ULifeDevourer::ULifeDevourer()
{
	Item_Name = "Devourer of Life";
	Item_Description = "+15.0% Lifesteal";
	Item_Description_Penalty = "-5.0 Fire Resistance";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset1/69.69'"));
	Item_Icon = IconObj.Object;
}

void ULifeDevourer::Apply_Effect()
{
	MyOwner->Lifesteal_Rate += 15.0f;

	MyOwner->FireResist_DMG -= 5.0f;
	MyOwner->FireResist_Item -= 5.0f;
}

void ULifeDevourer::Reverse_Effect()
{
	MyOwner->Lifesteal_Rate -= 15.0f;

	MyOwner->FireResist_DMG += 5.0f;
	MyOwner->FireResist_Item += 5.0f;
}