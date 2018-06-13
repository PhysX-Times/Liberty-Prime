// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LibertyPrimeCharacter.h"
#include "Knight.generated.h"

class ASword;
class UKnightColor;

UCLASS()
class LIBERTYPRIME_API AKnight : public ALibertyPrimeCharacter
{
	GENERATED_BODY()

public:

	AKnight();

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Long;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Medium;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float AttackDistance_Close;


	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		class ASword* MySword;

	void ChangeElemental(EDamageType ElementalType);

	virtual void RandomizeAppearance();
	USkeletalMesh* LoadMesh(TArray<FString> Paths);
	void SetMeshColor(UKnightColor* TargetMat);

	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Head, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Head;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Breastplate, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Breastplate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Shoulderpad, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Cape;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Breastplate, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Hood;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Arm, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Arm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Belt, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Belt;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WaistSide, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* WaistSide;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WaistBack, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* WaistBack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cloth, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* ClothFront;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cloth, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* ClothBack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Leg, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Leg;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LegDeco, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* LegDeco;
};
