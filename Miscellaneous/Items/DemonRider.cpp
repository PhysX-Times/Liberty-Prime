// Fill out your copyright notice in the Description page of Project Settings.

#include "DemonRider.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UDemonRider::UDemonRider()
{
	Item_Name = "Demon Rider of Night";
	Item_Description = "+50.0 Will Power \n+0.05 Frostbite Resistance";
	Item_Description_Penalty = "-8.0 Fire Resistance";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_blue_32.Tex_blue_32'"));
	Item_Icon = IconObj.Object;
}

void UDemonRider::Apply_Effect()
{
	MyOwner->MaxWillPower += 50.0f;
	MyOwner->MaxWillPower_Item += 50.0f;
	MyOwner->WillPower += 50.0f;

	MyOwner->FreezeAmount += 0.05f;
	MyOwner->Frostbite_Item += 0.05f;

	MyOwner->FireResist_DMG -= 8.0f;
	MyOwner->FireResist_Item -= 8.0f;
}

void UDemonRider::Reverse_Effect()
{
	MyOwner->MaxWillPower -= 50.0f;
	MyOwner->MaxWillPower_Item -= 50.0f;
	MyOwner->WillPower -= 50.0f;

	MyOwner->FreezeAmount -= 0.05f;
	MyOwner->Frostbite_Item -= 0.05f;

	MyOwner->FireResist_DMG += 8.0f;
	MyOwner->FireResist_Item += 8.0f;
}