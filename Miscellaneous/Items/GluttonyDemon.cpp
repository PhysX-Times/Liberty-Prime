// Fill out your copyright notice in the Description page of Project Settings.

#include "GluttonyDemon.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UGluttonyDemon::UGluttonyDemon()
{
	Item_Name = "Spectral Demon of Gluttony";
	Item_Description = "+25.0% Glory Gain";
	Item_Description_Penalty = "-5.0 Fire Resistance";
	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_violet_19.Tex_violet_19'"));
	Item_Icon = IconObj.Object;
}

void UGluttonyDemon::Apply_Effect()
{
	MyOwner->exp_multiplier += 0.25f;

	MyOwner->FireResist_DMG -= 5.0f;
	MyOwner->FireResist_Item -= 5.0f;
}

void UGluttonyDemon::Reverse_Effect()
{
	MyOwner->exp_multiplier -= 0.25f;

	MyOwner->FireResist_DMG += 5.0f;
	MyOwner->FireResist_Item += 5.0f;
}