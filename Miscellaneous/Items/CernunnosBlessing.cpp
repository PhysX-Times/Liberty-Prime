// Fill out your copyright notice in the Description page of Project Settings.

#include "CernunnosBlessing.h"
#include "Player/PlayerChar.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UCernunnosBlessing::UCernunnosBlessing()
{
	Item_Name = "Blessing of Cernunnos";
	Item_Description = "+0.5 Poison Damage \n+1.0 Poison Resistance";

	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_green_13.Tex_green_13'"));
	Item_Icon = IconObj.Object;
}

void UCernunnosBlessing::Apply_Effect()
{
	MyOwner->PoisonDMG += 0.5f;
	MyOwner->PoisonDMG_Item += 0.5f;

	MyOwner->PoisonResist_DMG += 1.0f;
	MyOwner->PoisonResist_Item += 1.0f;
}

void UCernunnosBlessing::Reverse_Effect()
{
	MyOwner->PoisonDMG -= 0.5f;
	MyOwner->PoisonDMG_Item -= 0.5f;

	MyOwner->PoisonResist_DMG -= 1.0f;
	MyOwner->PoisonResist_Item -= 1.0f;
}