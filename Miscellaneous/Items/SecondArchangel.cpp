// Fill out your copyright notice in the Description page of Project Settings.

#include "SecondArchangel.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

USecondArchangel::USecondArchangel()
{
	Item_Name = "Blessing of Second Archangel";
	Item_Description = "+25.0 Healing Potion Amount \n+30.0 Health";
	Item_Tier = 1;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset1/95.95'"));
	Item_Icon = IconObj.Object;
}

void USecondArchangel::Apply_Effect()
{
	MyOwner->potion_heal_amount += 25.0f;

	MyOwner->Update_Health_Ratio(30.0f);
}

void USecondArchangel::Reverse_Effect()
{
	MyOwner->potion_heal_amount -= 25.0f;

	MyOwner->Update_Health_Ratio(-30.0f);
}