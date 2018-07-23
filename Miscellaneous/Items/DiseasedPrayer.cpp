// Fill out your copyright notice in the Description page of Project Settings.

#include "DiseasedPrayer.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UDiseasedPrayer::UDiseasedPrayer()
{
	Item_Name = "Prayer of The Diseased";
	Item_Description = "+1.0 Poison Resistance";
	Item_Description_Penalty = "-20.0 Health \n-2.0 Physical Damage";

	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_green_22.Tex_green_22'"));
	Item_Icon = IconObj.Object;
}

void UDiseasedPrayer::Apply_Effect()
{
	MyOwner->PoisonResist_DMG += 1.0f;
	MyOwner->PoisonResist_Item += 1.0f;

	MyOwner->DMG -= 2.0f;
	MyOwner->DMG_Item -= 2.0f;

	MyOwner->Update_Health_Ratio(-20.0f);
}

void UDiseasedPrayer::Reverse_Effect()
{
	MyOwner->PoisonResist_DMG -= 1.0f;
	MyOwner->PoisonResist_Item -= 1.0f;

	MyOwner->DMG += 2.0f;
	MyOwner->DMG_Item += 2.0f;

	MyOwner->Update_Health_Ratio(20.0f);
}