// Fill out your copyright notice in the Description page of Project Settings.

#include "VenomBlade.h"
#include "Player/PlayerChar.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UVenomBlade::UVenomBlade()
{
	Item_Name = "Venom Blade";
	Item_Description = "+0.3 Poison Damage";

	Item_Tier = 1;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_violet_24.Tex_violet_24'"));
	Item_Icon = IconObj.Object;
}

void UVenomBlade::Apply_Effect()
{
	MyOwner->PoisonDMG += 0.3f;
	MyOwner->PoisonDMG_Item += 0.3f;
}

void UVenomBlade::Reverse_Effect()
{
	MyOwner->PoisonDMG -= 0.3f;
	MyOwner->PoisonDMG_Item -= 0.3f;
}