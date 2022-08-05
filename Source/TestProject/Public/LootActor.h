// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContainerActor.h"
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
		void GetLootItems();
	UFUNCTION()
		void GetRandomLootItems();
	UFUNCTION()
		void SetItemAmount(FSlotStructure Item, uint8 NewAmount);

	UFUNCTION()
		bool IsItemCurrency(const FSlotStructure Item);
	UFUNCTION()
		uint8 GetItemMaxStackSize(const FSlotStructure Item);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 MinLootItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 MaxLootItems;
};
