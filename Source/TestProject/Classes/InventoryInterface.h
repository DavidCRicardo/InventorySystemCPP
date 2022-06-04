// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EAttributes.h"
#include "Components/InventoryComponent.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORYSYSTEMCPP_API IInventoryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Default")
	void UI_UseInventoryItem(const uint8& InventorySlot);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Default")
	void UI_MoveInventoryItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Default")
	void UI_DropInventoryItem(const uint8& InventorySlot);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Default")
	void UI_EquipInventoryItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Default")
	void UI_UnEquipInventoryItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Default")
	void UI_TakeContainerItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Default")
	void GetContainerProperties(FName& Namee, uint8& SlotsPerRow, bool& IsStorageContainer, uint8& InventorySize);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Default")
	UInventoryComponent* GetContainerInventory(); 
};
