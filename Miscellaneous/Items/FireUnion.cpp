// Fill out your copyright notice in the Description page of Project Settings.

#include "FireUnion.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UFireUnion::UFireUnion()
{
	Item_Name = "Union of Fire Nations";
	Item_Description = "+10.0 Fire Damage \n+10.0 Fire Resistance \n+25.0 Health \n+25.0 Will Power";
	Item_Description_Penalty = "-0.05 Frostbite Resistance \n-0.2 Poison Resistance";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_yellow_37.Tex_yellow_37'"));
	Item_Icon = IconObj.Object;
}

void UFireUnion::Apply_Effect()
{
	MyOwner->Update_Health_Ratio(25.0f);

	MyOwner->Update_WillPower_Ratio(25.0f);

	MyOwner->FireDMG += 10.0f;
	MyOwner->FireDMG_Item += 10.0f;

	MyOwner->FireResist_DMG += 10.0f;
	MyOwner->FireResist_Item += 10.0f;

	MyOwner->FreezeResist_Frostbite -= 0.05f;
	MyOwner->FrostbiteResist_Item -= 0.05f;

	MyOwner->PoisonResist_DMG -= 0.2f;
	MyOwner->PoisonResist_Item -= 0.2f;
}

void UFireUnion::Reverse_Effect()
{
	MyOwner->Update_Health_Ratio(-25.0f);

	MyOwner->Update_WillPower_Ratio(-25.0f);

	MyOwner->FireDMG -= 10.0f;
	MyOwner->FireDMG_Item -= 10.0f;

	MyOwner->FireResist_DMG -= 10.0f;
	MyOwner->FireResist_Item -= 10.0f;

	MyOwner->FreezeResist_Frostbite += 0.05f;
	MyOwner->FrostbiteResist_Item += 0.05f;

	MyOwner->PoisonResist_DMG += 0.2f;
	MyOwner->PoisonResist_Item += 0.2f;
}