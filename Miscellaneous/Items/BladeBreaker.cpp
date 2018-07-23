// Fill out your copyright notice in the Description page of Project Settings.

#include "BladeBreaker.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UBladeBreaker::UBladeBreaker()
{
	Item_Name = "Blade Breaker";
	Item_Description = "+5.0 Physical Resistance";
	Item_Tier = 1;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset1/42.42'"));
	Item_Icon = IconObj.Object;
}

void UBladeBreaker::Apply_Effect()
{
	MyOwner->PhysicalResist_DMG += 5.0f;
	MyOwner->PhysicalResist_Item += 5.0f;
}

void UBladeBreaker::Reverse_Effect()
{
	MyOwner->PhysicalResist_DMG -= 5.0f;
	MyOwner->PhysicalResist_Item -= 5.0f;
}