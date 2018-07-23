// Fill out your copyright notice in the Description page of Project Settings.

#include "IceAge.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UIceAge::UIceAge()
{
	Item_Name = "Remnants of The Ice Age";
	Item_Description = "+0.02 Frostbite \n+0.02 Forstbite Resistance";
	Item_Tier = 1;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_emerald_4.Tex_emerald_4'"));
	Item_Icon = IconObj.Object;
}

void UIceAge::Apply_Effect()
{
	MyOwner->FreezeAmount += 0.02f;
	MyOwner->Frostbite_Item += 0.02f;

	MyOwner->FreezeResist_Frostbite += 0.02f;
	MyOwner->FrostbiteResist_Item += 0.02f;
}

void UIceAge::Reverse_Effect()
{
	MyOwner->FreezeAmount -= 0.02f;
	MyOwner->Frostbite_Item -= 0.02f;

	MyOwner->FreezeResist_Frostbite -= 0.02f;
	MyOwner->FrostbiteResist_Item -= 0.02f;
}