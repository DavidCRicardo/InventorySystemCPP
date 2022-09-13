// Fill out your copyright notice in the Description page of Project Settings.


#include "LootActor.h"
#include "Inventory/FLootList.h"
#include "MyGameInstance.h"

ALootActor::ALootActor()
{
	C_CanStoreItems = false;
}

bool ALootActor::InitializeInventory()
{
	if (HasAuthority())
	{
		InventoryItems = GetRandomLootItems();

		LoadInventoryItems(InventoryItems.Num(), InventoryItems);

		return true;
	}

	return false;
}

TArray<FSlotStructure> ALootActor::GetRandomLootItems()
{
	TArray<FSlotStructure> LootedItems{};

	if (HasAuthority())
	{
		uint8 LocalLootAmount = FMath::RandRange(MinLootItems, MaxLootItems);

		TArray<FLootList> LootLootItems = GetLootList();

		uint8 LocalLootCount = 0;
		uint8 LocalItemIndex = 0;
		TArray<uint8> LocalItemIndexes;

		if (!IsValid(DB_ItemList))
		{
			DB_ItemList = LoadObject<UDataTable>(this, TEXT("/Game/Blueprints/Item_DB.Item_DB"));
		}

		do
		{
			LocalItemIndex = FMath::RandRange(0, LootLootItems.Num() - 1);

			if (LocalItemIndexes.Find(LocalItemIndex) == -1)
			{
				FLootList Loot = LootLootItems[LocalItemIndex];

				if (Loot.DropChance >= FMath::RandRange(0.01f, 1.f))
				{
					LocalItemIndexes.AddUnique(LocalItemIndex);

					FItemStructure* LocalInventoryItem = DB_ItemList->FindRow<FItemStructure>(Loot.ID, "", true);

					FSlotStructure LocalInventorySlot{};
					LocalInventorySlot.InitSlot(*LocalInventoryItem, 0);

					uint8 LocalItemAmount = FMath::RandRange(Loot.MinAmount, Loot.MaxAmount);

					if (LocalItemAmount > GetItemMaxStackSize(LocalInventorySlot)) {
						if (!IsItemCurrency(LocalInventorySlot))
						{
							LocalItemAmount = GetItemMaxStackSize(LocalInventorySlot);
						}
					}

					SetItemAmount(LocalInventorySlot, LocalItemAmount);

					LootedItems.Add(LocalInventorySlot);

					LocalLootCount++;
				}
			}
		} while (LocalLootCount < LocalLootAmount);
	}

	return LootedItems;
}

TArray<FLootList> ALootActor::GetLootList()
{
	TArray<FLootList> LootItems{};

	if (!IsValid(BP_LootDB))
	{
		BP_LootDB = LoadObject<UDataTable>(this, TEXT("/Game/Blueprints/DT_LootList.DT_LootList"));
	}

	TArray<FName> LootData = BP_LootDB->GetRowNames();

	for (FName NameData : LootData)
	{
		FLootList* LootItem = BP_LootDB->FindRow<FLootList>(NameData, "", true);

		LootItems.Add(*LootItem);
	}

	return LootItems;
}

void ALootActor::SetItemAmount(FSlotStructure& Item, uint8 NewAmount)
{
	Item.Amount = NewAmount;
}

bool ALootActor::IsItemCurrency(const FSlotStructure Item)
{
	return Item.ItemStructure.ItemType == EItemType::Currency;
}

uint8 ALootActor::GetItemMaxStackSize(const FSlotStructure Item)
{
	return Item.ItemStructure.MaxStackSize;
}

void ALootActor::BeginPlay()
{
	FName InTableID = Cast<UMyGameInstance>(GetGameInstance())->COMMON_WORDS;
	Action = FText::FromStringTable(InTableID, "Loot");
	Name = FText::FromStringTable(InTableID, "Skeleton");

	DB_ItemList = LoadObject<UDataTable>(this, TEXT("/Game/Blueprints/Item_DB.Item_DB"));

	uint8 LocalNumberOfRows = InventoryComponent->NumberOfRowsInventory;
	uint8 LocalNumberOfSlotsPerRow = InventoryComponent->SlotsPerRowInventory;

	if (LocalNumberOfRows * LocalNumberOfSlotsPerRow < MaxLootItems)
	{
		for (uint8 i = 1; i < MaxLootItems; i++)
		{
			LocalNumberOfRows = i;
			LocalNumberOfSlotsPerRow = i;

			if (LocalNumberOfRows * LocalNumberOfSlotsPerRow >= MaxLootItems) {
				break;
			}
		}

		InventoryComponent->NumberOfRowsInventory = LocalNumberOfRows;
		InventoryComponent->SlotsPerRowInventory = LocalNumberOfSlotsPerRow;
	}

	Super::BeginPlay();
}