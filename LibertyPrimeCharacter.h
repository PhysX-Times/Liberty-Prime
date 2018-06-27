// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/TimelineComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "LibertyPrimeCharacter.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EDamageEnable : uint8
{
	Damage_RightHand 	UMETA(DisplayName = "Damage_RightHand"),
	Damage_LeftHand 	UMETA(DisplayName = "Damage_LeftHand"),
	Damage_None	UMETA(DisplayName = "Damage_None")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EDamageType : uint8
{
	Damage_Fire 	UMETA(DisplayName = "Damage_Fire"),
	Damage_Freeze 	UMETA(DisplayName = "Damage_Freeze"),
	Damage_Poison	UMETA(DisplayName = "Damage_Poison"),
	Damage_Lightning	UMETA(DisplayName = "Damage_Lightning"),
	Damage_None	UMETA(DisplayName = "Damage_None")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EICondition : uint8
{
	Condition_None	UMETA(DisplayName = "Condition_None"),
	Condition_Normal 	UMETA(DisplayName = "Condition_Normal"),
	Condition_Heavy 	UMETA(DisplayName = "Condition_Heavy"),
	Condition_Both 	UMETA(DisplayName = "Condition_Both")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EFaction : uint8
{
	Player 	UMETA(DisplayName = "Player"),
	Bot	UMETA(DisplayName = "Bot")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EWeaponType : uint8
{
	Weapon_Sharp 	UMETA(DisplayName = "Weapon_Sharp"),
	Weapon_Blunt 	UMETA(DisplayName = "Weapon_Blunt"),
	Weapon_Claw 	UMETA(DisplayName = "Weapon_Claw"),
	Weapon_None 	UMETA(DisplayName = "Weapon_None")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ERestriction : uint8
{
	Restriction_Full 	UMETA(DisplayName = "Restriction_Full"),
	Restriction_Fixed_Target	UMETA(DisplayName = "Restriction_Fixed_Target"),
	Restriction_Fixed_Rot	UMETA(DisplayName = "Restriction_Fixed_Rot"),
	Restriction_Rot 	UMETA(DisplayName = "Restriction_Rot"),
	Restriction_Move 	UMETA(DisplayName = "Restriction_Move"),
	Restriction_Move_NonRot 	UMETA(DisplayName = "Restriction_Move_NonRot"),
	Restriction_None	UMETA(DisplayName = "Restriction_None")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EImpact : uint8
{
	Impact_Light	UMETA(DisplayName = "Impact_Light"),
	Impact_Normal 	UMETA(DisplayName = "Impact_Normal"),
	Impact_Heavy 	UMETA(DisplayName = "Impact_Heavy")
};

class UAttackData;
class AWeapon;
class ASword;
class ALibertyPrimePlayerController;
class APathIndicator;
class ABloodDecal;

USTRUCT()
struct FMontageData
{
	GENERATED_BODY()

	FMontageData()
	{

	}

	FMontageData(UAnimMontage* TargetMontage_Par, float PlaySpeed_Par)
	{
		TargetMontage = TargetMontage_Par;
		PlaySpeed = PlaySpeed_Par;
	}

	UPROPERTY(EditAnywhere, Category = "MontageData")
		UAnimMontage* TargetMontage;
	UPROPERTY(EditAnywhere, Category = "MontageData")
		float PlaySpeed;
};

USTRUCT()
struct FDamageData
{
	GENERATED_BODY()

	UPROPERTY()
		class ALibertyPrimeCharacter* Applier;
	UPROPERTY()
		AActor* Source;
	UPROPERTY()
		class UAttackData* AttackData;
	UPROPERTY()
		EDamageType DMGType;
	UPROPERTY()
		EWeaponType WeaponType;
};

UCLASS(Blueprintable)
class ALibertyPrimeCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ALibertyPrimeCharacter();

	FString DeathLine_RefPath;

	ALibertyPrimeCharacter* FixedTarget;
	ALibertyPrimeCharacter* DamagerApplier;
	FRotator FixedRot;

	ALibertyPrimePlayerController* PlayerController;

	UPROPERTY(EditAnywhere, Category = "TargetPath")
		APathIndicator* TargetPath;

	ERestriction restriction;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	EFaction faction;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	EICondition push_cond;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	EICondition montage_stop_cond;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CppVariables")
	EDamageType DMGType;

	float Cap_Height;
	float MoveTo_Distance_Add;

	bool bBuildUp;
	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
	bool IsBlocking;
	bool IsDead;
	bool IsInvincible;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	bool IsRigidBody;
	bool bFocus;
	bool bIronWill;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	bool CanBeHeavyHit;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	bool CanHeavyHitRot;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	bool CanBeNormalHit;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	bool bHeavyInvincible;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	bool bHeavyRigidBody;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	bool bSpawnBlood;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	bool DoesBreathe;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	bool CanPlayFootstep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CppVariables")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CppVariables")
	float Health;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	float FootstepVolume;
	float RotLerpSpeed;
	float RotLerpSpeedNormal;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	float Weight;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	float DMG;
	float DMGOverTime;
	float DMGOverTime_Poison;
	float DMGOverTime_Fire;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	float FreezeAmount;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	float FreezeDuration;
	float FreezeTotal;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	float SpeedUpAmount;
	float SpeedUpTotal;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	float SpeedUpMax;
	float ElementalInterval;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	float FireDMG;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	float PoisonDMG;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	float LightningDMG;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	float WalkSpeed;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	float RunSpeed;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	float AttackDistance;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	float BloodSize_Decal;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	float BreatheInterval;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	float FootStepInterval;


	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float FreezeResist_Frostbite;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CppVariables")
		float FireResist_DMG;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float PoisonResist_DMG;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float PhysicalResist_DMG;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
		float LightningResist_DMG;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		int Critical_Chance;

	int Level;

	int DMGOverCount;
	int DMGOverCount_Current;

	int FireCount;
	int FireCount_Current;

	int PoisonCount;
	int PoisonCount_Current;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	int PoisonTick;
	UPROPERTY(EditAnywhere, Category = "CppVariables")
	int FireTick;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
		FVector BloodSize_PS;

	FRotator LerpRot;

	FTimerHandle DamagerTimer;
	FTimerHandle DamageOverTimer;
	FTimerHandle FireTimer;
	FTimerHandle FreezeTimer;
	FTimerHandle DilationTimer;
	FTimerHandle PoisonTimer;
	FTimerHandle LightningTimer;
	FTimerHandle BreatheTimer;
	FTimerHandle FootstepTimer;

	FTimeline PoisonLine;
	FTimeline IceLine;
	FTimeline FireLine;
	FTimeline LightningLine;
	FTimeline FresnelLine;
	FTimeline DeathLine;

	UPROPERTY()
		UCurveFloat* FresnelCurveFloat;
	UPROPERTY()
		UCurveFloat* ElementalCurveFloat;
	UPROPERTY()
		UCurveFloat* DeathCurveFloat;

	UPROPERTY(EditAnywhere, Category = "CppVariables")
	FLinearColor BloodColor;

	UPROPERTY()
		FLinearColor HealColor;
	UPROPERTY()
		FLinearColor PhysicalColor;
	UPROPERTY()
		FLinearColor FireColor;
	UPROPERTY()
		FLinearColor IceColor;
	UPROPERTY()
		FLinearColor PoisonColor;
	UPROPERTY()
		FLinearColor LightningColor;

	UPROPERTY()
		TArray<UParticleSystemComponent*> ParticleCluster;
	UPROPERTY()
		TArray<AWeapon*> WeaponCluster;
	
	UPROPERTY(EditAnywhere, Category = "SubClasses")
		TSubclassOf<ABloodDecal> BloodDecalClass;

	TArray<FName> ElementalPSLocs;

	int AttackSequenceCount = 0;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		class UAttackData* AttackData;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		TArray<ASword*> ActiveSensers;

	UPROPERTY(EditAnywhere, Category = "Montages")
		TArray<FMontageData> DeathDatas;
	UPROPERTY(EditAnywhere, Category = "Montages")
		TArray<FMontageData> HitDatas;
	UPROPERTY(EditAnywhere, Category = "Montages")
		TArray<FMontageData> HeavyHitDatas;

	UPROPERTY(EditAnywhere, Category = "ParticleEffect")
		UParticleSystem* HitPS;

	TArray<USkeletalMeshComponent*> BodyParts_SK;
	TArray<UStaticMeshComponent*> BodyParts_ST;

	UPROPERTY(EditAnywhere, Category = "BehaviorTree")
		class UBehaviorTree* BotBehavior;

	UPROPERTY()
		TArray<UAttackData*> RandomAttacks;

	UPROPERTY()
		UAudioComponent* SoundComponent_Active;

	UPROPERTY(BlueprintReadWrite, Category = "CppVariables")
		TArray<UAudioComponent*> SoundComponents;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_GetHit;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Hit_Blunt;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Hit_Sharp;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Hit_Claw;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Death;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Attack;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* SoundCue_Breathe;

	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* Footstep_Concrete;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* Footstep_Dirt;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* Footstep_Grass;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* Footstep_Gravel;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* Footstep_Snow;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* Footstep_Mud;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* Footstep_Metal;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* Footstep_Water;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* Footstep_Sand;
	UPROPERTY(EditAnywhere, Category = "SoundCues")
		USoundCue* Footstep_Wood;

	UPROPERTY(EditAnywhere, Category = "ParticleEffects")
		UParticleSystem* FireCriticalPS;
	UPROPERTY(EditAnywhere, Category = "ParticleEffects")
		UParticleSystem* IceCriticalPS;
	UPROPERTY(EditAnywhere, Category = "ParticleEffects")
		UParticleSystem* PoisonCriticalPS;
	UPROPERTY(EditAnywhere, Category = "ParticleEffects")
		UParticleSystem* LightningCriticalPS;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SpawnBloodDecal(FVector SourceLoc, FVector ImpactLoc);

	void Walk_Start();
	void Walk_Stop();

	void Breathe();

	FDamageData BakeDamageData(AActor* Source, UAttackData* AttackData, EDamageType DMGType, EWeaponType WeaponType);

	virtual void RandomAttack();
	UAttackData* AttackData_Create(FString path, USoundCue* TargetSoundCue, ERestriction restriction, EImpact impact, float DMGMultiplier, float RotSpeed, float PlaySpeed, bool IsSequencer);
	UAttackData* AttackData_Create(USoundCue* TargetSoundCue, EImpact impact, float DMGMultiplier);
	void RandomAttackData_Create(FString path, TArray<UAttackData*> &AttackData_arr_ref, USoundCue* TargetSoundCue, ERestriction restriction, EImpact impact, float DMGMultiplier, float RotSpeed, float PlaySpeed, bool IsSequencer);
	void RandomAttackData_Create(TArray<FString> &paths, USoundCue* TargetSoundCue, ERestriction restriction, EImpact impact, float DMGMultiplier, float RotSpeed, float PlaySpeed, bool IsSequencer);
	void RandomAttackData_Create(FString path, USoundCue* TargetSoundCue, ERestriction restriction, EImpact impact, float DMGMultiplier, float RotSpeed, float PlaySpeed, bool IsSequencer);
	void Attack(UAttackData* TargetData);
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		virtual void AttackCheck();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		virtual void AttackSequenceCheck();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void StartRot();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void EndRot();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void StartFly();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void EndFly();

	bool CheckRestriction();

	void ApplyElemental(ALibertyPrimeCharacter* Applier, EDamageType DMGType);

	UFUNCTION()
		void PoisonFunction(float Val);
	UFUNCTION()
		void IceFunction(float Val);
	UFUNCTION()
		void FireFunction(float Val);
	UFUNCTION()
		void LightningFunction(float Val);
	UFUNCTION()
		void FresnelFunction(float Val);
	UFUNCTION()
		void DeathFunction(float Val);

	void ApplyFunction(FName ParamName, float Val);
	void ApplyFunction(FName ParamName, FLinearColor Val);
	void InitializeFunction(FTimeline* Timeline, FName functionName, UCurveFloat* CurveFloat);
	void TL_CheckTick(FTimeline* Timeline, float DeltaTime);

	void Fire(int Count, float DmgVal);
	void Fire_Apply();
	void FireReset();

	void Poison(int Count, float DmgVal);
	void Poison_Clear();
	void Poison_Apply();

	void Light_SpeedUp();
	void Light_SpeedDown();

	void Freeze(int Count, float Multiplier);
	void FreezeReset();

	void CheckDilation();

	UFUNCTION(BlueprintNativeEvent, Category = "CppFunctions")
		void UpdateHealth_PB();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void ActiveSensers_Add(ASword* TargetSword);

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		UAudioComponent* Play_SoundCue(USoundCue* TargetSoundCue, bool ClearClustered = false, FName AttachName = FName("None"), float volume = 1.0f, USceneComponent* Attach_Comp = nullptr);
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		UAudioComponent* Play_SoundCue_Location(USoundCue* TargetSoundCue, FVector TargetLoc, float volume = 1.0f, bool ClearClustered = false);

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void Play_FootStep();

	void FootStep_Reset();

	void SoundActive_Destroy();

	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		void ResetSequenceCount();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		virtual void ResetDamager(ASword* TargetSenser);
	virtual void ResetDamager();
	virtual void ResetDamager_Add();

	virtual void RotLerp(float DeltaTime);

	virtual void IronWill_Start();
	virtual void IronWill_End();

	virtual void Build_Up();

	virtual void MassDamage(float radius, UAttackData* AttackData, FVector TraceLoc, EWeaponType WeaponType, AActor* Source = nullptr, bool bPassDMGType = false, EDamageType DMGType_Param = EDamageType::Damage_None);
	virtual void Damager(FDamageData DamageData, FVector PSLoc);
	virtual void Damager_Simple(float DMG, FLinearColor Color);
	virtual void DamageOverTime(float Damage, int Count);
	virtual void DamageOverTime_Apply();
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
	virtual void Die();
	virtual void Die_Add();
	virtual void PlayMontage(UAnimMontage* TargetMontage, float PlaySpeed, ERestriction restriction, float TargetRotSpeed);
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
		virtual void ResetMontage();
	virtual void ResetMontage_Add();
	virtual void PlayMontage_Add();
};