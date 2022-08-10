// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContainerActor.h"
#include "Inventory/FLootList.h"
#include "LootActor.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEMCPP_API ALootActor : public AContainerActor
{
	GENERATED_BODY()

public:
	ALootActor();

	virtual bool InitializeInventory() override;

	UFUNCTION()
		TArray<FLootList> GetLootList();
	UFUNCTION()
		TArray<FSlotStructure> GetRandomLootItems();
	UFUNCTION()
		void SetItemAmount(FSlotStructure Item, uint8 NewAmount);

	UFUNCTION()
		bool IsItemCurrency(const FSlotStructure Item);
	UFUNCTION()
		uint8 GetItemMaxStackSize(const FSlotStructure Item);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 MinLootItems = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 MaxLootItems = 4;

protected:
	virtual void BeginPlay() override;

private:
	TArray<FSlotStructure> InventoryItems;

	UDataTable* DB_ItemList;

	UDataTable* BP_LootDB;
};