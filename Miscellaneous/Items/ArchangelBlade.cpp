// Fill out your copyright notice in the Description page of Project Settings.

#include "ArchangelBlade.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UArchangelBlade::UArchangelBlade()
{
	Item_Name = "Blade of First Archangel";
	Item_Description = "+50.0 Health \n+5.0 Physical Damage \n+10.0 Fire Damage";
	Item_Description_Penalty = "-0.02 Frostbite Resistance \n-8.0 Lightning Resistance";
	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_yellow_22.Tex_yellow_22'"));
	Item_Icon = IconObj.Object;
}

void UArchangelBlade::Apply_Effect()
{
	MyOwner->Update_Health_Ratio(50.0f);

	MyOwner->DMG += 5.0f;
	MyOwner->DMG_Item += 5.0f;

	MyOwner->FireDMG += 10.0f;
	MyOwner->FireDMG_Item += 10.0f;

	MyOwner->FreezeResist_Frostbite -= 0.02f;
	MyOwner->FrostbiteResist_Item -= 0.02f;

	MyOwner->LightningResist_DMG -= 8.0f;
	MyOwner->LightningResist_Item -= 8.0f;
}

void UArchangelBlade::Reverse_Effect()
{
	MyOwner->Update_Health_Ratio(-50.0f);

	MyOwner->DMG -= 5.0f;
	MyOwner->DMG_Item -= 5.0f;

	MyOwner->FireDMG -= 10.0f;
	MyOwner->FireDMG_Item -= 10.0f;

	MyOwner->FreezeResist_Frostbite += 0.02f;
	MyOwner->FrostbiteResist_Item += 0.02f;

	MyOwner->LightningResist_DMG += 8.0f;
	MyOwner->LightningResist_Item += 8.0f;
}