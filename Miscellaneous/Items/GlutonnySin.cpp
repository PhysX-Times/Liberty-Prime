// Fill out your copyright notice in the Description page of Project Settings.

#include "GlutonnySin.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UGlutonnySin::UGlutonnySin()
{
	Item_Name = "Sin of Gluttony";
	Item_Description = "+10.0% Lifesteal";
	Item_Description_Penalty = "-10.0% Glory Gain";
	Item_Tier = 1;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_red_29.Tex_red_29'"));
	Item_Icon = IconObj.Object;
}

void UGlutonnySin::Apply_Effect()
{
	MyOwner->Lifesteal_Rate += 10.0f;

	MyOwner->exp_multiplier -= 0.1f;
}

void UGlutonnySin::Reverse_Effect()
{
	MyOwner->Lifesteal_Rate -= 10.0f;

	MyOwner->exp_multiplier += 0.1f;
}