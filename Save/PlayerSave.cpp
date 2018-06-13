// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerSave.h"
#include "Player/PlayerChar.h"
#include "Miscellaneous/Item.h"

UPlayerSave::UPlayerSave()
{
	UserIndex = 0;
	SaveSlotName = "PlayerSave";
	slot_stages.Init(0, 9);
}