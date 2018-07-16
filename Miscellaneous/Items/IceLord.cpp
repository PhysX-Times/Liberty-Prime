// Fill out your copyright notice in the Description page of Project Settings.

#include "IceLord.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UIceLord::UIceLord()
{
	Item_Name = "Conquerer of The Ice Age";
	Item_Description = "+0.05 Frostbite \n+0.07 Frostbite Resistance";
	Item_Description_Penalty = "-5.0 Fire Resistance";
	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset1/15.15'"));
	Item_Icon = IconObj.Object;
}

void UIceLord::Apply_Effect()
{
	MyOwner->FreezeAmount += 0.05f;
	MyOwner->Frostbite_Item += 0.05f;

	MyOwner->FreezeResist_Frostbite += 0.07f;
	MyOwner->FrostbiteResist_Item += 0.07f;

	MyOwner->FireResist_DMG -= 5.0f;
	MyOwner->FireResist_Item -= 5.0f;
}

void UIceLord::Reverse_Effect()
{
	MyOwner->FreezeAmount -= 0.05f;
	MyOwner->Frostbite_Item -= 0.05f;

	MyOwner->FreezeResist_Frostbite -= 0.07f;
	MyOwner->FrostbiteResist_Item -= 0.07f;

	MyOwner->FireResist_DMG += 5.0f;
	MyOwner->FireResist_Item += 5.0f;
}