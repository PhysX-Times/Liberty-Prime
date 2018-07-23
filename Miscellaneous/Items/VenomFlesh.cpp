// Fill out your copyright notice in the Description page of Project Settings.

#include "VenomFlesh.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UVenomFlesh::UVenomFlesh()
{
	Item_Name = "Venom Flesh";
	Item_Description = "Immunity to Poison Damage";
	Item_Description_Penalty = "-100.0 Health";
	Item_Tier = 3;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_violet_23.Tex_violet_23'"));
	Item_Icon = IconObj.Object;
}

void UVenomFlesh::Apply_Effect()
{
	MyOwner->bPoisonImmunity = true;

	MyOwner->Update_Health_Ratio(-100.0f);
}

void UVenomFlesh::Reverse_Effect()
{
	MyOwner->bPoisonImmunity = false;

	MyOwner->Update_Health_Ratio(100.0f);
}