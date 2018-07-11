// Fill out your copyright notice in the Description page of Project Settings.

#include "ToxicDefense.h"
#include "Player/PlayerChar.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UToxicDefense::UToxicDefense()
{
	Item_Name = "Toxic Shell";
	Item_Description = "+1.0 Poison Resistance";

	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_violet_15.Tex_violet_15'"));
	Item_Icon = IconObj.Object;
}

void UToxicDefense::Apply_Effect()
{
	MyOwner->PoisonResist_DMG += 1.0f;
	MyOwner->PoisonResist_Item += 1.0f;
}

void UToxicDefense::Reverse_Effect()
{
	MyOwner->PoisonResist_DMG -= 1.0f;
	MyOwner->PoisonResist_Item -= 1.0f;
}