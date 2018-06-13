// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "MusicManager.generated.h"

UCLASS()
class LIBERTYPRIME_API AMusicManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMusicManager();

	UPROPERTY(EditAnywhere, Category = "Music")
		USoundCue* BackGroundMusic;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float Volume;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
