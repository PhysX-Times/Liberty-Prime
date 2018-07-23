// Fill out your copyright notice in the Description page of Project Settings.

#include "BoneBreaker.h"
#include "Player/PlayerChar.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

UBoneBreaker::UBoneBreaker()
{
	Item_Name = "Bone Breaker";
	Item_Description = "+5.0 Physical Damage";
	Item_Tier = 1;

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconObj(TEXT("Texture2D'/Game/UI/Icons/Iconset3/Tex_yellow_3.Tex_yellow_3'"));
	Item_Icon = IconObj.Object;
}

void UBoneBreaker::Apply_Effect()
{
	MyOwner->DMG += 5.0f;
	MyOwner->DMG_Item += 5.0f;
}

void UBoneBreaker::Reverse_Effect()
{
	MyOwner->DMG -= 5.0f;
	MyOwner->DMG_Item -= 5.0f;
}