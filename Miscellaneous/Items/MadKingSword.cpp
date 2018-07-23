// Fill out your copyright notice in the Description page of Project Settings.

#include "MadKingSword.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UMadKingSword::UMadKingSword()
{
	Item_Name = "Cursed Sword of Mad King";
	Item_Description = "+10.0 Physical Damage \n+0.5 Poison Damage";
	Item_Description_Penalty = "-50.0 Health \n-5.0% Glory Gain";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_violet_4.Tex_violet_4'"));
	Item_Icon = IconObj.Object;
}

void UMadKingSword::Apply_Effect()
{
	MyOwner->DMG += 10.0f;
	MyOwner->DMG_Item += 10.0f;

	MyOwner->PoisonDMG += 0.5f;
	MyOwner->PoisonDMG_Item += 0.5f;

	MyOwner->Update_Health_Ratio(-50.0f);

	MyOwner->exp_multiplier -= 0.05f;
}

void UMadKingSword::Reverse_Effect()
{
	MyOwner->DMG -= 10.0f;
	MyOwner->DMG_Item -= 10.0f;

	MyOwner->PoisonDMG -= 0.5f;
	MyOwner->PoisonDMG_Item -= 0.5f;

	MyOwner->Update_Health_Ratio(50.0f);

	MyOwner->exp_multiplier += 0.05f;
}