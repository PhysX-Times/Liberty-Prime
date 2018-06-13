// Fill out your copyright notice in the Description page of Project Settings.

#include "Inferno.h"
#include "Player/PlayerChar.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UInferno::UInferno()
{
	Item_Name = "Inferno";
	Item_Description = "Fire Damage: +5 \nFire Resistance: +3";

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset1/106.106'"));
	Item_Icon = IconObj.Object;
}

void UInferno::Apply_Effect()
{
	MyOwner->FireDMG += 5.0f;
	MyOwner->FireResist_DMG += 3.0f;
}

void UInferno::Reverse_Effect()
{
	MyOwner->FireDMG -= 5.0f;
	MyOwner->FireResist_DMG -= 3.0f;
}