// Fill out your copyright notice in the Description page of Project Settings.

#include "ToxicForestCurse.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UToxicForestCurse::UToxicForestCurse()
{
	Item_Name = "Curse of Toxic Forest";
	Item_Description = "+1.5 Poison Damage \n+1.0 Poison Resistance";
	Item_Description_Penalty = "-100.0 Health";

	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_green_26.Tex_green_26'"));
	Item_Icon = IconObj.Object;
}

void UToxicForestCurse::Apply_Effect()
{
	MyOwner->PoisonDMG += 1.5f;
	MyOwner->PoisonDMG_Item += 1.5f;

	MyOwner->PoisonResist_DMG += 1.0f;
	MyOwner->PoisonResist_Item += 1.0f;

	MyOwner->MaxHealth -= 100.0f;
	MyOwner->MaxHealth_Item -= 100.0f;
	MyOwner->Health -= 100.0f;
}

void UToxicForestCurse::Reverse_Effect()
{
	MyOwner->PoisonDMG -= 1.5f;
	MyOwner->PoisonDMG_Item -= 1.5f;

	MyOwner->PoisonResist_DMG -= 1.0f;
	MyOwner->PoisonResist_Item -= 1.0f;

	MyOwner->MaxHealth += 100.0f;
	MyOwner->MaxHealth_Item += 100.0f;
	MyOwner->Health += 100.0f;
}