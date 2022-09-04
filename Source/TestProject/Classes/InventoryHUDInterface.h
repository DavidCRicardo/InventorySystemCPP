// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryHUDInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryHUDInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class INVENTORYSYSTEMCPP_API IInventoryHUDInterface
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
		//
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default")
		void UI_MoveInventoryItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default")
		void UI_DropInventoryItem(const uint8& InventorySlot);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default")
		void UI_EquipInventoryItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default")
		void UI_UnEquipInventoryItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default")
		void UI_UseInventoryItem(const uint8& InventorySlot);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default")
		void UI_UseContainerItem(const uint8& InventorySlot);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default")
		void UI_TakeContainerItem(const uint8& FromContainerSlot, const uint8& ToInventorySlot);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default")
		void UI_DepositContainerItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default")
		void UI_EquipFromContainer(const uint8& FromInventorySlot, const uint8& ToInventorySlot);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default")
		void UI_UnEquipToContainer(const uint8& FromInventorySlot, const uint8& ToInventorySlot);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default")
		void UI_MoveHotbarItem(const uint8& FromSlot, const uint8& ToSlot, const bool IsDraggedFromInventory, const bool IsDraggedFromHotbar);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default")
		void UI_MoveContainerItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot);
};