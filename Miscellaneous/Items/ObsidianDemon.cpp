// Fill out your copyright notice in the Description page of Project Settings.

#include "ObsidianDemon.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UObsidianDemon::UObsidianDemon()
{
	Item_Name = "Terror of Obsidian Overlord";
	Item_Description = "+50.0 Health \n+5.0 Physical Damage \n+5.0 Physical Resistance";
	Item_Description_Penalty = "-0.05 Frostbite Resistance";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_yellow_25.Tex_yellow_25'"));
	Item_Icon = IconObj.Object;
}

void UObsidianDemon::Apply_Effect()
{
	MyOwner->MaxHealth += 50.0f;
	MyOwner->MaxHealth_Item += 50.0f;
	MyOwner->Health += 50.0f;

	MyOwner->DMG += 5.0f;
	MyOwner->DMG_Item += 5.0f;

	MyOwner->PhysicalResist_DMG += 5.0f;
	MyOwner->PhysicalResist_Item += 5.0f;

	MyOwner->FreezeResist_Frostbite -= 0.05f;
	MyOwner->FrostbiteResist_Item -= 0.05f;
}

void UObsidianDemon::Reverse_Effect()
{
	MyOwner->MaxHealth -= 50.0f;
	MyOwner->MaxHealth_Item -= 50.0f;
	MyOwner->Health -= 50.0f;

	MyOwner->DMG -= 5.0f;
	MyOwner->DMG_Item -= 5.0f;

	MyOwner->PhysicalResist_DMG -= 5.0f;
	MyOwner->PhysicalResist_Item -= 5.0f;

	MyOwner->FreezeResist_Frostbite += 0.05f;
	MyOwner->FrostbiteResist_Item += 0.05f;
}