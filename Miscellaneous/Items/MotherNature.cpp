// Fill out your copyright notice in the Description page of Project Settings.

#include "MotherNature.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UMotherNature::UMotherNature()
{
	Item_Name = "Gift of The Mother Nature";
	Item_Description = "+50.0 Healing Potion Amount \n+75.0 Health \n+0.3 Poison Resistance";
	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_green_10.Tex_green_10'"));
	Item_Icon = IconObj.Object;
}

void UMotherNature::Apply_Effect()
{
	MyOwner->Update_Health_Ratio(75.0f);

	MyOwner->potion_heal_amount += 50.0f;

	MyOwner->PoisonResist_DMG += 0.3f;
	MyOwner->PoisonResist_Item += 0.3f;
}

void UMotherNature::Reverse_Effect()
{
	MyOwner->Update_Health_Ratio(-75.0f);

	MyOwner->potion_heal_amount -= 50.0f;

	MyOwner->PoisonResist_DMG -= 0.3f;
	MyOwner->PoisonResist_Item -= 0.3f;
}