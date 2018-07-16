// Fill out your copyright notice in the Description page of Project Settings.

#include "FogHill.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UFogHill::UFogHill()
{
	Item_Name = "Dance of Fog Hill";
	Item_Description = "+1.0 Physical Resistance \n+2.0 Fire Resistance \n+4.0 Lightning Resistance \n+0.1 Poison Resistance";
	Item_Description_Penalty = "-5.0 Fire Resistance";
	Item_Tier = 1;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset1/16.16'"));
	Item_Icon = IconObj.Object;
}

void UFogHill::Apply_Effect()
{
	MyOwner->PhysicalResist_DMG += 1.0f;
	MyOwner->PhysicalResist_Item += 1.0f;

	MyOwner->FireResist_DMG += 2.0f;
	MyOwner->FireResist_Item += 2.0f;

	MyOwner->LightningResist_DMG += 4.0f;
	MyOwner->LightningResist_Item += 4.0f;

	MyOwner->PoisonResist_DMG += 0.1f;
	MyOwner->PoisonResist_Item += 0.1f;
}

void UFogHill::Reverse_Effect()
{
	MyOwner->PhysicalResist_DMG -= 1.0f;
	MyOwner->PhysicalResist_Item -= 1.0f;

	MyOwner->FireResist_DMG -= 2.0f;
	MyOwner->FireResist_Item -= 2.0f;

	MyOwner->LightningResist_DMG -= 4.0f;
	MyOwner->LightningResist_Item -= 4.0f;

	MyOwner->PoisonResist_DMG -= 0.1f;
	MyOwner->PoisonResist_Item -= 0.1f;
}