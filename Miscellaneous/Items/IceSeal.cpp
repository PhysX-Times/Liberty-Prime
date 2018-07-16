// Fill out your copyright notice in the Description page of Project Settings.

#include "IceSeal.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UIceSeal::UIceSeal()
{
	Item_Name = "Ice Seal";
	Item_Description = "+0.05 Frostbite \n+0.01 Frostbite Resistance";
	Item_Description_Penalty = "-15.0 Will Power";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_ff_08.Tex_ff_08'"));
	Item_Icon = IconObj.Object;
}

void UIceSeal::Apply_Effect()
{
	MyOwner->FreezeAmount += 0.05f;
	MyOwner->Frostbite_Item += 0.05f;
	MyOwner->FreezeResist_Frostbite += 0.01f;
	MyOwner->FrostbiteResist_Item += 0.01f;

	MyOwner->MaxWillPower -= 15.0f;
	MyOwner->MaxWillPower_Item -= 15.0f;
	MyOwner->WillPower -= 15.0f;
}

void UIceSeal::Reverse_Effect()
{
	MyOwner->FreezeAmount -= 0.05f;
	MyOwner->Frostbite_Item -= 0.05f;
	MyOwner->FreezeResist_Frostbite -= 0.01f;
	MyOwner->FrostbiteResist_Item -= 0.01f;

	MyOwner->MaxWillPower += 15.0f;
	MyOwner->MaxWillPower_Item += 15.0f;
	MyOwner->WillPower += 15.0f;
}