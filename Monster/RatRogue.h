// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Monster.h"
#include "RatRogue.generated.h"

class ASword;
class UAttackData;

UCLASS()
class LIBERTYPRIME_API ARatRogue : public AMonster
{
	GENERATED_BODY()
	
public:

	ARatRogue();

	UPROPERTY(EditAnywhere, Category = "SkeletalMeshes")
		USkeletalMesh* Mesh_1;
	UPROPERTY(EditAnywhere, Category = "SkeletalMeshes")
		USkeletalMesh* Mesh_2;
	UPROPERTY(EditAnywhere, Category = "SkeletalMeshes")
		USkeletalMesh* Mesh_3;

	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ASword> SwordDamagerClass;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* LeftSwordDamager;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		ASword* RightSwordDamager;


	virtual void BeginPlay() override;
	
};
