// Fill out your copyright notice in the Description page of Project Settings.

#include "GateGuardian.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UGateGuardian::UGateGuardian()
{
	Item_Name = "Guardian of Underworld Gates";
	Item_Description = "+50.0 Health \n+5.0% Glory Gain \n+10.0 Physical Damage \n+0.5 Poison Damage";
	Item_Description_Penalty = "-10.0 Fire Damage \n-20.0 Lightning Damage";
	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_violet_11.Tex_violet_11'"));
	Item_Icon = IconObj.Object;
}

void UGateGuardian::Apply_Effect()
{
	MyOwner->Update_Health_Ratio(50.0f);

	MyOwner->exp_multiplier += 0.05f;

	MyOwner->DMG += 10.0f;
	MyOwner->DMG_Item += 10.0f;

	MyOwner->PoisonDMG += 0.5f;
	MyOwner->PoisonDMG_Item += 0.5f;

	MyOwner->FireDMG -= 10.0f;
	MyOwner->FireDMG_Item -= 10.0f;

	MyOwner->LightningDMG -= 20.0f;
	MyOwner->LightningDMG_Item -= 20.0f;
}

void UGateGuardian::Reverse_Effect()
{
	MyOwner->Update_Health_Ratio(-50.0f);

	MyOwner->exp_multiplier -= 0.05f;

	MyOwner->DMG -= 10.0f;
	MyOwner->DMG_Item -= 10.0f;

	MyOwner->PoisonDMG -= 0.5f;
	MyOwner->PoisonDMG_Item -= 0.5f;

	MyOwner->FireDMG += 10.0f;
	MyOwner->FireDMG_Item += 10.0f;

	MyOwner->LightningDMG += 20.0f;
	MyOwner->LightningDMG_Item += 20.0f;
}