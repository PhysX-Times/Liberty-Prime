// Fill out your copyright notice in the Description page of Project Settings.

#include "PainTolerance.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UPainTolerance::UPainTolerance()
{
	Item_Name = "Pain Tolerance";
	Item_Description = "+1.0 Physical Resistance \n+0.1 Poison Resistance \n+2.0 Fire Resistance \n+4.0 Lightning Resistance";
	Item_Tier = 1;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_red_22.Tex_red_22'"));
	Item_Icon = IconObj.Object;
}

void UPainTolerance::Apply_Effect()
{
	MyOwner->PhysicalResist_DMG += 1.0f;
	MyOwner->PhysicalResist_Item += 1.0f;

	MyOwner->PoisonResist_DMG += 0.1f;
	MyOwner->PoisonResist_Item += 0.1f;

	MyOwner->FireResist_DMG += 2.0f;
	MyOwner->FireResist_Item += 2.0f;

	MyOwner->LightningResist_DMG += 4.0f;
	MyOwner->LightningResist_Item += 4.0f;
}

void UPainTolerance::Reverse_Effect()
{
	MyOwner->PhysicalResist_DMG -= 1.0f;
	MyOwner->PhysicalResist_Item -= 1.0f;

	MyOwner->PoisonResist_DMG -= 0.1f;
	MyOwner->PoisonResist_Item -= 0.1f;

	MyOwner->FireResist_DMG -= 2.0f;
	MyOwner->FireResist_Item -= 2.0f;

	MyOwner->LightningResist_DMG -= 4.0f;
	MyOwner->LightningResist_Item -= 4.0f;
}