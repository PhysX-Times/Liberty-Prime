// Fill out your copyright notice in the Description page of Project Settings.

#include "BloodLust.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UBloodLust::UBloodLust()
{
	Item_Name = "Bloodlust";
	Item_Description = "+5% Lifesteal \n+5.0 Physical Damage";
	Item_Description_Penalty = "-5.0% Glory Gain";
	Item_Tier = 1;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset1/89.89'"));
	Item_Icon = IconObj.Object;
}

void UBloodLust::Apply_Effect()
{
	MyOwner->Lifesteal_Rate += 5.0f;

	MyOwner->DMG += 5.0f;
	MyOwner->DMG_Item += 5.0f;

	MyOwner->exp_multiplier -= 0.5f;
}

void UBloodLust::Reverse_Effect()
{
	MyOwner->Lifesteal_Rate -= 5.0f;

	MyOwner->DMG -= 5.0f;
	MyOwner->DMG_Item -= 5.0f;

	MyOwner->exp_multiplier += 0.5f;
}