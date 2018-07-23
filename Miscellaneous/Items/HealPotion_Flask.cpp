// Fill out your copyright notice in the Description page of Project Settings.

#include "HealPotion_Flask.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UHealPotion_Flask::UHealPotion_Flask()
{
	Item_Name = "Flask of Healing Potion";
	Item_Description = "+2 Healing Potion at Sanctuary";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_red_29.Tex_red_29'"));
	Item_Icon = IconObj.Object;
}

void UHealPotion_Flask::Apply_Effect()
{
	MyOwner->potion_heal_max += 2;
	MyOwner->potion_heal_count += 2;

	MyOwner->Update_PotionCount_Health();
}

void UHealPotion_Flask::Reverse_Effect()
{
	MyOwner->potion_heal_max += 2;
	MyOwner->potion_heal_count += 2;

	MyOwner->Update_PotionCount_Health();
}