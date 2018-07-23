// Fill out your copyright notice in the Description page of Project Settings.

#include "UndeadFortitude.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UUndeadFortitude::UUndeadFortitude()
{
	Item_Name = "Undead Fortitude";
	Item_Description = "+50.0 Health \n+5.0 Physical Resistance";
	Item_Description_Penalty = "-25.0 Will Power";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_ff_03.Tex_ff_03'"));
	Item_Icon = IconObj.Object;
}

void UUndeadFortitude::Apply_Effect()
{
	MyOwner->Update_Health_Ratio(50.0f);

	MyOwner->Update_WillPower_Ratio(-25.0);

	MyOwner->PhysicalResist_DMG += 5.0f;
	MyOwner->PhysicalResist_Item += 5.0f;
}

void UUndeadFortitude::Reverse_Effect()
{
	MyOwner->Update_Health_Ratio(-50.0f);

	MyOwner->Update_WillPower_Ratio(25.0);

	MyOwner->PhysicalResist_DMG -= 5.0f;
	MyOwner->PhysicalResist_Item -= 5.0f;
}