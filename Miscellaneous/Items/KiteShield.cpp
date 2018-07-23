// Fill out your copyright notice in the Description page of Project Settings.

#include "KiteShield.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UKiteShield::UKiteShield()
{
	Item_Name = "Kite Shield of Treachery";
	Item_Description = "+10.0% Shield Block Damage";
	Item_Description_Penalty = "-35.0 Health";
	Item_Tier = 1;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset1/41.41'"));
	Item_Icon = IconObj.Object;
}

void UKiteShield::Apply_Effect()
{
	MyOwner->shield_block_plus += 0.1f;

	MyOwner->Update_Health_Ratio(-35.0f);
}

void UKiteShield::Reverse_Effect()
{
	MyOwner->shield_block_plus -= 0.1f;

	MyOwner->Update_Health_Ratio(35.0f);
}