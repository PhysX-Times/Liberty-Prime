// Fill out your copyright notice in the Description page of Project Settings.

#include "LifeThief.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

ULifeThief::ULifeThief()
{
	Item_Name = "Thief of Life";
	Item_Description = "+20.0% Lifesteal";
	Item_Description_Penalty = "-40.0 Health";
	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_emerald_7.Tex_emerald_7'"));
	Item_Icon = IconObj.Object;
}

void ULifeThief::Apply_Effect()
{
	MyOwner->Lifesteal_Rate += 20.0f;

	MyOwner->Update_Health_Ratio(-40.0f);
}

void ULifeThief::Reverse_Effect()
{
	MyOwner->Lifesteal_Rate -= 20.0f;

	MyOwner->Update_Health_Ratio(40.0f);
}