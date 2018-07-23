// Fill out your copyright notice in the Description page of Project Settings.

#include "SlothSin.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

USlothSin::USlothSin()
{
	Item_Name = "Sin of Sloth";
	Item_Description = "+75.0 Health";
	Item_Description_Penalty = "-10.0% Glory Gain";
	Item_Tier = 1;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_yellow_32.Tex_yellow_32'"));
	Item_Icon = IconObj.Object;
}

void USlothSin::Apply_Effect()
{
	MyOwner->Update_Health_Ratio(75.0);

	MyOwner->exp_multiplier -= 0.1f;
}

void USlothSin::Reverse_Effect()
{
	MyOwner->Update_Health_Ratio(-75.0);

	MyOwner->exp_multiplier += 0.1f;
}