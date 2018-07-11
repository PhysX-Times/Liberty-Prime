// Fill out your copyright notice in the Description page of Project Settings.

#include "PoisonDefense.h"
#include "Player/PlayerChar.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UPoisonDefense::UPoisonDefense()
{
	Item_Name = "Poison Shell";
	Item_Description = "+0.5 Poison Resistance";

	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_violet_13.Tex_violet_13'"));
	Item_Icon = IconObj.Object;
}

void UPoisonDefense::Apply_Effect()
{
	MyOwner->PoisonResist_DMG += 0.5f;
	MyOwner->PoisonResist_Item += 0.5f;
}

void UPoisonDefense::Reverse_Effect()
{
	MyOwner->PoisonResist_DMG -= 0.5f;
	MyOwner->PoisonResist_Item -= 0.5f;
}