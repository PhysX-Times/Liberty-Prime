// Fill out your copyright notice in the Description page of Project Settings.

#include "BruteForce.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UBruteForce::UBruteForce()
{
	Item_Name = "Brute Force for Fools";
	Item_Description = "+4.0 Physical Damage";
	Item_Tier = 1;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_red_12.Tex_red_12'"));
	Item_Icon = IconObj.Object;
}

void UBruteForce::Apply_Effect()
{
	MyOwner->DMG += 4.0f;
	MyOwner->DMG_Item += 4.0f;
}

void UBruteForce::Reverse_Effect()
{
	MyOwner->DMG -= 4.0f;
	MyOwner->DMG_Item -= 4.0f;
}