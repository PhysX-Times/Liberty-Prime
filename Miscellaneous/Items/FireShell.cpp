// Fill out your copyright notice in the Description page of Project Settings.

#include "FireShell.h"
#include "Player/PlayerChar.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UFireShell::UFireShell()
{
	Item_Name = "Inferno's Shell";
	Item_Description = "+10.0 Fire Resistance";

	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_yellow_40.Tex_yellow_40'"));
	Item_Icon = IconObj.Object;
}

void UFireShell::Apply_Effect()
{
	MyOwner->FireResist_DMG += 10.0f;
	MyOwner->FireResist_Item += 10.0f;
}

void UFireShell::Reverse_Effect()
{
	MyOwner->FireResist_DMG -= 10.0f;
	MyOwner->FireResist_Item -= 10.0f;
}