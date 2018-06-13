// Fill out your copyright notice in the Description page of Project Settings.

#include "SnSKnight_Bot.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon/Sword.h"

ASnSKnight_Bot::ASnSKnight_Bot()
{
	faction = EFaction::Bot;
	Weight = 67.5f;
	Health = 100.0f;
	MaxHealth = 100.0f;
}

void ASnSKnight_Bot::BeginPlay()
{
	Super::BeginPlay();

	FTransform SpawnTrans;
	SpawnTrans.SetLocation(GetActorLocation());
	SpawnTrans.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
	SpawnTrans.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));

	MySword = GetWorld()->SpawnActorDeferred<ASword>(SwordClass, SpawnTrans, this, Instigator);
	MySword->MyOwner = this;
	UGameplayStatics::FinishSpawningActor(MySword, SpawnTrans);
	MySword->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("SwordSocket"));

	WeaponCluster.Add(MySword);

	int rand = UKismetMathLibrary::RandomIntegerInRange(1, 4);

	switch (rand)
	{
	case 1:
		ChangeElemental(EDamageType::Damage_Fire);
		break;
	case 2:
		ChangeElemental(EDamageType::Damage_Freeze);
		break;
	case 3:
		ChangeElemental(EDamageType::Damage_Poison);
		break;
	case 4:
		ChangeElemental(EDamageType::Damage_Lightning);
		break;
	}
}