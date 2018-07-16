// Fill out your copyright notice in the Description page of Project Settings.

#include "ForestSpirit.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UForestSpirit::UForestSpirit()
{
	Item_Name = "Spirits of Ancient Forest";
	Item_Description = "+120.0 Will Power";
	Item_Description_Penalty = "-30.0 Health";
	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_blue_26.Tex_blue_26'"));
	Item_Icon = IconObj.Object;
}

void UForestSpirit::Apply_Effect()
{
	MyOwner->MaxWillPower += 120.0f;
	MyOwner->MaxWillPower_Item += 120.0f;
	MyOwner->WillPower += 120.0f;

	MyOwner->MaxHealth -= 30.0f;
	MyOwner->MaxHealth_Item -= 30.0f;
	MyOwner->Health -= 30.0f;
}

void UForestSpirit::Reverse_Effect()
{
	MyOwner->MaxWillPower -= 120.0f;
	MyOwner->MaxWillPower_Item -= 120.0f;
	MyOwner->WillPower -= 120.0f;

	MyOwner->MaxHealth += 30.0f;
	MyOwner->MaxHealth_Item += 30.0f;
	MyOwner->Health += 30.0f;
}