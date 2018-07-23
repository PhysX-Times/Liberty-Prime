// Fill out your copyright notice in the Description page of Project Settings.

#include "IceUnion.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UIceUnion::UIceUnion()
{
	Item_Name = "Union of Ice Nations";
	Item_Description = "+0.05 Frostbite \n+0.05 Frostbite Resistance \n+35.0 Health \n+5.0 Physical Damage";
	Item_Description_Penalty = "-5.0 Fire Damage \n-10.0 Fire Resistance";
	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_blue_10.Tex_blue_10'"));
	Item_Icon = IconObj.Object;
}

void UIceUnion::Apply_Effect()
{
	MyOwner->Update_Health_Ratio(35.0f);

	MyOwner->FreezeAmount += 0.05f;
	MyOwner->Frostbite_Item += 0.05f;

	MyOwner->FreezeResist_Frostbite += 0.05f;
	MyOwner->FrostbiteResist_Item += 0.05f;

	MyOwner->DMG += 5.0f;
	MyOwner->DMG_Item += 5.0f;

	MyOwner->FireDMG -= 5.0f;
	MyOwner->FireDMG_Item -= 5.0f;

	MyOwner->FireResist_DMG -= 10.0f;
	MyOwner->FireResist_Item -= 10.0f;
}

void UIceUnion::Reverse_Effect()
{
	MyOwner->Update_Health_Ratio(-35.0f);

	MyOwner->FreezeAmount -= 0.05f;
	MyOwner->Frostbite_Item -= 0.05f;

	MyOwner->FreezeResist_Frostbite -= 0.05f;
	MyOwner->FrostbiteResist_Item -= 0.05f;

	MyOwner->DMG -= 5.0f;
	MyOwner->DMG_Item -= 5.0f;

	MyOwner->FireDMG += 5.0f;
	MyOwner->FireDMG_Item += 5.0f;

	MyOwner->FireResist_DMG += 10.0f;
	MyOwner->FireResist_Item += 10.0f;
}