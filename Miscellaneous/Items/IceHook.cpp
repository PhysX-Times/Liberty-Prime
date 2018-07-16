// Fill out your copyright notice in the Description page of Project Settings.

#include "IceHook.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UIceHook::UIceHook()
{
	Item_Name = "Mystic Ice Hook";
	Item_Description = "+0.05 Frostbite";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_blue_38.Tex_blue_38'"));
	Item_Icon = IconObj.Object;
}

void UIceHook::Apply_Effect()
{
	MyOwner->FreezeAmount += 0.05f;
	MyOwner->Frostbite_Item += 0.05f;
}

void UIceHook::Reverse_Effect()
{
	MyOwner->FreezeAmount -= 0.05f;
	MyOwner->Frostbite_Item -= 0.05f;
}