// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "LibertyPrimePlayerController.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "LibertyPrimeCharacter.h"
#include "Player/PlayerChar.h"
#include "Engine.h"

ALibertyPrimePlayerController::ALibertyPrimePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void ALibertyPrimePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	APlayerChar* MyPawn = Cast<APlayerChar>(GetPawn());

	for (auto DMGIndicatorData : DMGIndicatorDatas)
	{
		FVector2D TargetPos;
		UGameplayStatics::ProjectWorldToScreen(this, DMGIndicatorData.InitLocation, TargetPos);
		DMGIndicatorData.InitWidget->SetPositionInViewport(TargetPos - FVector2D(22.5f, 0.0f));
	}

	if (MyPawn)
	{
		float MouseX;
		float MouseY;
		GetMousePosition(MouseX, MouseY);
		FVector2D MousePosition(MouseX, MouseY);

		FVector2D ScreenLoc;
		UGameplayStatics::ProjectWorldToScreen(this, MyPawn->GetActorLocation(), ScreenLoc);

		float DP = FVector2D::DotProduct(ScreenLoc.GetSafeNormal(), MousePosition.GetSafeNormal());
		float Degree = FMath::RadiansToDegrees(FMath::Acos(DP));

		float Ang1 = FMath::Atan2(ScreenLoc.X, ScreenLoc.Y);
		float Ang2 = FMath::Atan2(MousePosition.X - ScreenLoc.X, MousePosition.Y - ScreenLoc.Y);
		float Ang = FMath::RadiansToDegrees(Ang1 - Ang2);

		MyPawn->LerpRot = FRotator(0.0f, Ang + 5.0f, 0.0f);
	}
}

void ALibertyPrimePlayerController::Add_Indicator(float Val, FLinearColor Color, FVector Location)
{
	UUserWidget* new_indicator = CreateWidget<UUserWidget>(GetWorld(), DMGIndicator_Class);
	new_indicator->AddToViewport();
	FVector2D NewPos;
	UGameplayStatics::ProjectWorldToScreen(this, Location, NewPos);
	new_indicator->SetPositionInViewport(NewPos - FVector2D(22.5f, 0.0f));
	DMGIndicatorDatas.Add(FIndicatorData(new_indicator, Location));

	Indicator_Event(new_indicator, Val, Color);
}

void ALibertyPrimePlayerController::Indicator_Event_Implementation(UUserWidget* TargetWidget, float Val, FLinearColor Color)
{

}