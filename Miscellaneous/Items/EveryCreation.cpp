// Fill out your copyright notice in the Description page of Project Settings.

#include "EveryCreation.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UEveryCreation::UEveryCreation()
{
	Item_Name = "Creation of Everything";
	Item_Description = "+75.0 Health \n+ 25.0 Will Power \n+5% Lifesteal";
	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_emerald_1.Tex_emerald_1'"));
	Item_Icon = IconObj.Object;
}

void UEveryCreation::Apply_Effect()
{
	MyOwner->Lifesteal_Rate += 5.0f;

	MyOwner->Update_Health_Ratio(75.0f);

	MyOwner->Update_WillPower_Ratio(25.0f);
}

void UEveryCreation::Reverse_Effect()
{
	MyOwner->Lifesteal_Rate -= 5.0f;

	MyOwner->Update_Health_Ratio(-75.0f);

	MyOwner->Update_WillPower_Ratio(-25.0f);
}