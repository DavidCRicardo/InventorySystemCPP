// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

class UInventoryComponent;
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
	void GetContainerProperties(FName& Namee, uint8& NumberOfRows, uint8& SlotsPerRow, bool& IsStorageContainer, uint8& InventorySize);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Default")
	void GetContainerInventory(UInventoryComponent*& OutInventoryComponent);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Default")
	TArray<APlayerState*> GetPlayersViewing();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default")
	bool GetCanStoreItems();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Default")
	bool ContainerLooted();
};