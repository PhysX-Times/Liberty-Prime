// Fill out your copyright notice in the Description page of Project Settings.

#include "LoneWolf.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

ULoneWolf::ULoneWolf()
{
	Item_Name = "Virtue of Lone Wolf";
	Item_Description = "+50.0 Will Power \n+0.2 Will Power Refresh Rate";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_blue_29.Tex_blue_29'"));
	Item_Icon = IconObj.Object;
}

void ULoneWolf::Apply_Effect()
{
	MyOwner->WillPower_Rate += 0.2f;

	MyOwner->Update_WillPower_Ratio(50.0f);

}

void ULoneWolf::Reverse_Effect()
{
	MyOwner->WillPower_Rate -= 0.2f;

	MyOwner->Update_WillPower_Ratio(-50.0f);
}