// Fill out your copyright notice in the Description page of Project Settings.

#include "VirtueFortitude.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UVirtueFortitude::UVirtueFortitude()
{
	Item_Name = "Virtue of Fortitude";
	Item_Description = "+25.0 Health \n+1.0 Physical Resistance \n+2.0 Fire Resistance \n+0.1 Poison Resistance \n+4.0 Lightning Resistance";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_yellow_31.Tex_yellow_31'"));
	Item_Icon = IconObj.Object;
}

void UVirtueFortitude::Apply_Effect()
{
	MyOwner->Update_Health_Ratio(25.0f);

	MyOwner->PhysicalResist_DMG += 1.0f;
	MyOwner->PhysicalResist_Item += 1.0f;

	MyOwner->FireResist_DMG += 2.0f;
	MyOwner->FireResist_Item += 2.0f;

	MyOwner->PoisonResist_DMG += 0.1f;
	MyOwner->PoisonResist_Item += 0.1f;

	MyOwner->LightningResist_DMG += 4.0f;
	MyOwner->LightningResist_Item += 4.0f;
}

void UVirtueFortitude::Reverse_Effect()
{
	MyOwner->Update_Health_Ratio(-25.0f);

	MyOwner->PhysicalResist_DMG -= 1.0f;
	MyOwner->PhysicalResist_Item -= 1.0f;

	MyOwner->FireResist_DMG -= 2.0f;
	MyOwner->FireResist_Item -= 2.0f;

	MyOwner->PoisonResist_DMG -= 0.1f;
	MyOwner->PoisonResist_Item -= 0.1f;

	MyOwner->LightningResist_DMG -= 4.0f;
	MyOwner->LightningResist_Item -= 4.0f;
}