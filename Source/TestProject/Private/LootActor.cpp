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

void ALootActor::GetLootItems()
{
}

void ALootActor::GetRandomLootItems()
{
}

void ALootActor::SetItemAmount(FSlotStructure Item, uint8 NewAmount)
{
	Item.Amount = NewAmount;
}

bool ALootActor::IsItemCurrency(const FSlotStructure Item)
{
	return false;
}

uint8 ALootActor::GetItemMaxStackSize(const FSlotStructure Item)
{
	return Item.ItemStructure.MaxStackSize;
}
