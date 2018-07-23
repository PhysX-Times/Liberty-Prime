// Fill out your copyright notice in the Description page of Project Settings.

#include "DeathTouch.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UDeathTouch::UDeathTouch()
{
	Item_Name = "Touch of Death";
	Item_Description = "+10.0 Physical Damage \n+4.0 Fire Damage \n+0.2 Poison Damage \n+8.0 Lightning Damage";
	Item_Description_Penalty = "-50.0 Health";
	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_ff_10.Tex_ff_10'"));
	Item_Icon = IconObj.Object;
}

void UDeathTouch::Apply_Effect()
{
	MyOwner->DMG += 10.0f;
	MyOwner->DMG_Item += 10.0f;

	MyOwner->FireDMG += 4.0f;
	MyOwner->FireDMG_Item += 4.0f;

	MyOwner->PoisonDMG += 0.2f;
	MyOwner->PoisonDMG_Item += 0.2f;

	MyOwner->LightningDMG += 8.0f;
	MyOwner->LightningDMG_Item += 8.0f;

	MyOwner->Update_Health_Ratio(-50.0f);
}

void UDeathTouch::Reverse_Effect()
{
	MyOwner->DMG -= 10.0f;
	MyOwner->DMG_Item -= 10.0f;

	MyOwner->FireDMG -= 4.0f;
	MyOwner->FireDMG_Item -= 4.0f;

	MyOwner->PoisonDMG -= 0.2f;
	MyOwner->PoisonDMG_Item -= 0.2f;

	MyOwner->LightningDMG -= 8.0f;
	MyOwner->LightningDMG_Item -= 8.0f;

	MyOwner->Update_Health_Ratio(50.0f);
}