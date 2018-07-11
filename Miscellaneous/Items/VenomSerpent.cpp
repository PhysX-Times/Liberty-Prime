// Fill out your copyright notice in the Description page of Project Settings.

#include "VenomSerpent.h"
#include "Player/PlayerChar.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UVenomSerpent::UVenomSerpent()
{
	Item_Name = "Venom Serpent";
	Item_Description = "+0.5 Poison Damage";

	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_green_21.Tex_green_21'"));
	Item_Icon = IconObj.Object;
}

void UVenomSerpent::Apply_Effect()
{
	MyOwner->PoisonDMG += 0.5f;
	MyOwner->PoisonDMG_Item += 0.5f;
}

void UVenomSerpent::Reverse_Effect()
{
	MyOwner->PoisonDMG -= 0.5f;
	MyOwner->PoisonDMG_Item -= 0.5f;
}