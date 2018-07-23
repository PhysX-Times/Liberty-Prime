// Fill out your copyright notice in the Description page of Project Settings.

#include "FireWolf.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UFireWolf::UFireWolf()
{
	Item_Name = "Wrath of The Fire Wolf";
	Item_Description = "+12.0 Fire Damage \n+8.0 Fire Resistance";
	Item_Description_Penalty = "-10.0 Health";
	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset1/103.103'"));
	Item_Icon = IconObj.Object;
}

void UFireWolf::Apply_Effect()
{
	MyOwner->FireDMG += 12.0f;
	MyOwner->FireDMG_Item += 12.0f;

	MyOwner->FireResist_DMG += 8.0f;
	MyOwner->FireResist_Item += 8.0f;

	MyOwner->Update_Health_Ratio(-10.0f);
}

void UFireWolf::Reverse_Effect()
{
	MyOwner->FireDMG -= 12.0f;
	MyOwner->FireDMG_Item -= 12.0f;

	MyOwner->FireResist_DMG -= 8.0f;
	MyOwner->FireResist_Item -= 8.0f;

	MyOwner->Update_Health_Ratio(10.0f);
}