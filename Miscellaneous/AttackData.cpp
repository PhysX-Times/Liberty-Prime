// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackData.h"

UAttackData::UAttackData()
{
	PlaySpeed = 1.0f;
	DMGMultiplier = 1.0f;
	RotSpeed = 2.7f;
	impact = EImpact::Impact_Normal;
	IsSequencer = false;
	restriction = ERestriction::Restriction_Full;
}