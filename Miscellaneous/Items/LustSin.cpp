// Fill out your copyright notice in the Description page of Project Settings.

#include "LustSin.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

ULustSin::ULustSin()
{
	Item_Name = "Sin of Lust";
	Item_Description = "+3.0 Physical Damage \n+6.0 FireDamage \n+0.3 Poison Damage \n+12.0 Lightning Damage";
	Item_Description_Penalty = "-50.0 Health";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_ff_07.Tex_ff_07'"));
	Item_Icon = IconObj.Object;
}

void ULustSin::Apply_Effect()
{
	MyOwner->DMG += 3.0f;
	MyOwner->DMG_Item += 3.0f;

	MyOwner->FireDMG += 6.0f;
	MyOwner->FireDMG_Item += 6.0f;

	MyOwner->PoisonDMG += 0.3f;
	MyOwner->PoisonDMG_Item += 0.3f;

	MyOwner->LightningDMG += 12.0f;
	MyOwner->LightningDMG_Item += 12.0f;

	MyOwner->Update_Health_Ratio(-50.0f);
}

void ULustSin::Reverse_Effect()
{
	MyOwner->DMG -= 3.0f;
	MyOwner->DMG_Item -= 3.0f;

	MyOwner->FireDMG -= 6.0f;
	MyOwner->FireDMG_Item -= 6.0f;

	MyOwner->PoisonDMG -= 0.3f;
	MyOwner->PoisonDMG_Item -= 0.3f;

	MyOwner->LightningDMG -= 12.0f;
	MyOwner->LightningDMG_Item -= 12.0f;

	MyOwner->Update_Health_Ratio(50.0f);
}