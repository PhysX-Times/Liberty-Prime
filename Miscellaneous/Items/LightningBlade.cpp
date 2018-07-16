// Fill out your copyright notice in the Description page of Project Settings.

#include "LightningBlade.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

ULightningBlade::ULightningBlade()
{
	Item_Name = "Lightning Blade";
	Item_Description = "+40.0 Lightning Damage";
	Item_Description_Penalty = "+10% Return Lightning Damage";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset1/48.48'"));
	Item_Icon = IconObj.Object;
}

void ULightningBlade::Apply_Effect()
{
	MyOwner->LightningDMG += 40.0f;
	MyOwner->LightningDMG_Item += 40.0f;

	MyOwner->Lightning_Percentage += 10.0f;
}

void ULightningBlade::Reverse_Effect()
{
	MyOwner->LightningDMG -= 40.0f;
	MyOwner->LightningDMG_Item -= 40.0f;

	MyOwner->Lightning_Percentage -= 10.0f;
}