// Fill out your copyright notice in the Description page of Project Settings.

#include "HeartCreation.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UHeartCreation::UHeartCreation()
{
	Item_Name = "Heart of Creation";
	Item_Description = "+100.0 Health \n+25.0 Will Power \n+5.0 Physical Damage \n+4.0 Physical Resistance";
	Item_Description_Penalty = "-1.0 Poison Resistance";
	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_violet_26.Tex_violet_26'"));
	Item_Icon = IconObj.Object;
}

void UHeartCreation::Apply_Effect()
{
	MyOwner->MaxHealth += 100.0f;
	MyOwner->MaxHealth_Item += 100.0f;
	MyOwner->Health += 100.0f;

	MyOwner->MaxWillPower += 25.0f;
	MyOwner->MaxWillPower_Item += 25.0f;
	MyOwner->WillPower += 25.0f;

	MyOwner->DMG += 5.0f;
	MyOwner->DMG_Item += 5.0f;

	MyOwner->PhysicalResist_DMG += 4.0f;
	MyOwner->PhysicalResist_Item += 4.0f;

	MyOwner->PoisonResist_DMG -= 1.0f;
	MyOwner->PoisonResist_Item -= 1.0f;
}

void UHeartCreation::Reverse_Effect()
{
	MyOwner->MaxHealth -= 100.0f;
	MyOwner->MaxHealth_Item -= 100.0f;
	MyOwner->Health -= 100.0f;

	MyOwner->MaxWillPower -= 25.0f;
	MyOwner->MaxWillPower_Item -= 25.0f;
	MyOwner->WillPower -= 25.0f;

	MyOwner->DMG -= 5.0f;
	MyOwner->DMG_Item -= 5.0f;

	MyOwner->PhysicalResist_DMG -= 4.0f;
	MyOwner->PhysicalResist_Item -= 4.0f;

	MyOwner->PoisonResist_DMG += 1.0f;
	MyOwner->PoisonResist_Item += 1.0f;
}