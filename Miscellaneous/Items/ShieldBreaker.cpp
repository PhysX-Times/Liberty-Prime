// Fill out your copyright notice in the Description page of Project Settings.

#include "ShieldBreaker.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UShieldBreaker::UShieldBreaker()
{
	Item_Name = "Shield Breaker";
	Item_Description = "+8.0 Physical Damage";
	Item_Description_Penalty = "-25.0 Health";
	Item_Tier = 1;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset1/43.43'"));
	Item_Icon = IconObj.Object;
}

void UShieldBreaker::Apply_Effect()
{
	MyOwner->DMG += 8.0f;
	MyOwner->DMG_Item += 8.0f;

	MyOwner->Update_Health_Ratio(-25.0f);
}

void UShieldBreaker::Reverse_Effect()
{
	MyOwner->DMG -= 8.0f;
	MyOwner->DMG_Item -= 8.0f;

	MyOwner->Update_Health_Ratio(25.0f);
}