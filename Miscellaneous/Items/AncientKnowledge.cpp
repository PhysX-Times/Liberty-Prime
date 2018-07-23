// Fill out your copyright notice in the Description page of Project Settings.

#include "AncientKnowledge.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UAncientKnowledge::UAncientKnowledge()
{
	Item_Name = "Ancient Knowledge";
	Item_Description = "+10.0% Glory Gain \n+35.0 Health \n+35.0 Will Power";
	Item_Description_Penalty = "-5.0 Physical Resistance";
	Item_Tier = 2;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_yellow_41.Tex_yellow_41'"));
	Item_Icon = IconObj.Object;
}

void UAncientKnowledge::Apply_Effect()
{
	MyOwner->Update_Health_Ratio(35.0f);

	MyOwner->Update_WillPower_Ratio(35.0f);

	MyOwner->exp_multiplier += 0.1f;

	MyOwner->PhysicalResist_DMG -= 5.0f;
	MyOwner->PhysicalResist_Item -= 5.0f;
}

void UAncientKnowledge::Reverse_Effect()
{
	MyOwner->Update_Health_Ratio(-35.0f);

	MyOwner->Update_WillPower_Ratio(-35.0f);

	MyOwner->exp_multiplier -= 0.1f;

	MyOwner->PhysicalResist_DMG += 5.0f;
	MyOwner->PhysicalResist_Item += 5.0f;
}