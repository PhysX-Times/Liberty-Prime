// Fill out your copyright notice in the Description page of Project Settings.

#include "Inferno.h"
#include "Player/PlayerChar.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UInferno::UInferno()
{
	Item_Name = "Inferno";
	Item_Description = "+10.0 Fire Damage \n+5.0 Fire Resistance";

	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset1/106.106'"));
	Item_Icon = IconObj.Object;
}

void UInferno::Apply_Effect()
{
	MyOwner->FireDMG += 10.0f;
	MyOwner->FireDMG_Item += 10.0f;

	MyOwner->FireResist_DMG += 5.0f;
	MyOwner->FireResist_Item += 5.0f;
}

void UInferno::Reverse_Effect()
{
	MyOwner->FireDMG -= 10.0f;
	MyOwner->FireDMG_Item -= 10.0f;

	MyOwner->FireResist_DMG -= 5.0f;
	MyOwner->FireResist_Item -= 5.0f;
}