// Fill out your copyright notice in the Description page of Project Settings.

#include "LifeDestruction.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

ULifeDestruction::ULifeDestruction()
{
	Item_Name = "Annihilation";
	Item_Description = "+5.0 Physical Damage \n+10.0 Fire Damage \n+10.0 Lightning Damage";
	Item_Description_Penalty = "-50.0 Health";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset1/9.9'"));
	Item_Icon = IconObj.Object;
}

void ULifeDestruction::Apply_Effect()
{
	MyOwner->DMG += 5.0f;
	MyOwner->DMG_Item += 5.0f;

	MyOwner->FireDMG += 10.0f;
	MyOwner->FireDMG_Item += 10.0f;

	MyOwner->LightningDMG += 10.0f;
	MyOwner->LightningDMG_Item += 10.0f;

	MyOwner->Update_Health_Ratio(-50.0f);
}

void ULifeDestruction::Reverse_Effect()
{
	MyOwner->DMG -= 5.0f;
	MyOwner->DMG_Item -= 5.0f;

	MyOwner->FireDMG -= 10.0f;
	MyOwner->FireDMG_Item -= 10.0f;

	MyOwner->LightningDMG -= 10.0f;
	MyOwner->LightningDMG_Item -= 10.0f;

	MyOwner->Update_Health_Ratio(50.0f);
}