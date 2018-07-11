// Fill out your copyright notice in the Description page of Project Settings.

#include "PoisonousDemonTerror.h"
#include "Player/PlayerChar.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UPoisonousDemonTerror::UPoisonousDemonTerror()
{
	Item_Name = "Terror of Venom Demon";
	Item_Description = "+0.5 Poison Damage \n+0.5 Fire Resistance";

	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_violet_21.Tex_violet_21'"));
	Item_Icon = IconObj.Object;
}

void UPoisonousDemonTerror::Apply_Effect()
{
	MyOwner->PoisonDMG += 0.5f;
	MyOwner->PoisonDMG_Item += 0.5f;

	MyOwner->PoisonResist_DMG += 0.5f;
	MyOwner->PoisonResist_Item += 0.5f;
}

void UPoisonousDemonTerror::Reverse_Effect()
{
	MyOwner->PoisonDMG -= 0.5f;
	MyOwner->PoisonDMG_Item -= 0.5f;

	MyOwner->PoisonResist_DMG -= 0.5f;
	MyOwner->PoisonResist_Item -= 0.5f;
}