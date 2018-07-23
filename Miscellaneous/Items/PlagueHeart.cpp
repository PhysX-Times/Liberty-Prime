// Fill out your copyright notice in the Description page of Project Settings.

#include "PlagueHeart.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UPlagueHeart::UPlagueHeart()
{
	Item_Name = "Plague Heart";
	Item_Description = "+50.0 Health \n+0.2 Poison Resistance";
	Item_Description_Penalty = "-1.0 Physical Damage";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_red_24.Tex_red_24'"));
	Item_Icon = IconObj.Object;
}

void UPlagueHeart::Apply_Effect()
{
	MyOwner->Update_Health_Ratio(50.0f);

	MyOwner->PoisonResist_DMG += 0.2f;
	MyOwner->PoisonResist_Item += 0.2f;

	MyOwner->DMG -= 1.0f;
	MyOwner->DMG_Item -= 1.0f;
}

void UPlagueHeart::Reverse_Effect()
{
	MyOwner->Update_Health_Ratio(-50.0f);

	MyOwner->PoisonResist_DMG -= 0.2f;
	MyOwner->PoisonResist_Item -= 0.2f;

	MyOwner->DMG += 1.0f;
	MyOwner->DMG_Item += 1.0f;
}