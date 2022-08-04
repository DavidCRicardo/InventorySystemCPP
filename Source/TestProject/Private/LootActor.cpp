// Fill out your copyright notice in the Description page of Project Settings.


#include "LootActor.h"

ALootActor::ALootActor()
{
	MinLootItems = 2;
	MaxLootItems = 4;

	C_InventorySize = 4;
	C_SlotsPerRow = 1;
	C_CanStoreItems = false;

	Action = LOCTABLE(COMMON_WORDS2, "Loot");
}

bool ALootActor::InitializeInventory()
{
	return false;
}
