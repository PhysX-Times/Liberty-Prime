// Fill out your copyright notice in the Description page of Project Settings.

#include "ForestSpirit.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UForestSpirit::UForestSpirit()
{
	Item_Name = "Spirits of Ancient Forest";
	Item_Description = "+50.0 Health";
	Item_Description_Penalty = "-20.0 Will Power \n-2.0 Physical Damage";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_blue_26.Tex_blue_26'"));
	Item_Icon = IconObj.Object;
}

void UForestSpirit::Apply_Effect()
{
	MyOwner->PoisonResist_DMG += 1.0f;
	MyOwner->DMG -= 2.0f;
	MyOwner->MaxHealth -= 20.0f;
	MyOwner->Health -= 20.0f;
}

void UForestSpirit::Reverse_Effect()
{
	MyOwner->PoisonResist_DMG -= 1.0f;
	MyOwner->DMG += 2.0f;
	MyOwner->MaxHealth += 20.0f;
	MyOwner->Health += 20.0f;
}