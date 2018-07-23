// Fill out your copyright notice in the Description page of Project Settings.

#include "MysticPhantom.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UMysticPhantom::UMysticPhantom()
{
	Item_Name = "Gift from Mystic Phantom";
	Item_Description = "+25.0 Health \n+25.0 Will Power \n+5.0 Physical Resistance \n+0.2 Poison Resistance";
	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_violet_7.Tex_violet_7'"));
	Item_Icon = IconObj.Object;
}

void UMysticPhantom::Apply_Effect()
{
	MyOwner->Update_WillPower_Ratio(25.0f);

	MyOwner->Update_Health_Ratio(25.0f);

	MyOwner->PoisonResist_DMG += 0.2f;
	MyOwner->PoisonResist_Item += 0.2f;

	MyOwner->PhysicalResist_DMG += 5.0f;
	MyOwner->PhysicalResist_Item += 5.0f;
}

void UMysticPhantom::Reverse_Effect()
{
	MyOwner->Update_WillPower_Ratio(-25.0f);

	MyOwner->Update_Health_Ratio(-25.0f);

	MyOwner->PoisonResist_DMG -= 0.2f;
	MyOwner->PoisonResist_Item -= 0.2f;

	MyOwner->PhysicalResist_DMG -= 5.0f;
	MyOwner->PhysicalResist_Item -= 5.0f;
}