// Fill out your copyright notice in the Description page of Project Settings.

#include "PlateArmor.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UPlateArmor::UPlateArmor()
{
	Item_Name = "Chestplate of Imminent Dreams";
	Item_Description = "+25.0 Health \n+5.0 Physical Resistance";
	Item_Tier = 1;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset1/49.49'"));
	Item_Icon = IconObj.Object;
}

void UPlateArmor::Apply_Effect()
{
	MyOwner->Update_Health_Ratio(25.0f);

	MyOwner->PhysicalResist_DMG += 5.0;
	MyOwner->PhysicalResist_Item += 5.0;
}

void UPlateArmor::Reverse_Effect()
{
	MyOwner->Update_Health_Ratio(-25.0f);

	MyOwner->PhysicalResist_DMG -= 5.0;
	MyOwner->PhysicalResist_Item -= 5.0;
}