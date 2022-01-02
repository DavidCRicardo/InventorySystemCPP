// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WindowLayout.h"
#include "Inventory_Slot.h"
#include "InventoryLayout.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UInventoryLayout : public UWindowLayout
{
	GENERATED_BODY()

public:
	UInventoryLayout();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UUniformGridPanel* InventoryGridPanel;
	
	UPROPERTY(meta = (BindWidget))
	UButton* SortButton;
	UPROPERTY()
	UTexture2D* Background_Slot;

	UFUNCTION()
	void RefreshInventorySlots();
	
protected:
	virtual void NativeConstruct() override;

	virtual void OnButtonQuitClicked() override;
	
	void InitializeInventorySlots();

	UPROPERTY()
	TArray<UInventory_Slot*>InventorySlotsArray;
	
private:
	UPROPERTY(EditAnywhere, Category = "Default")
	TSubclassOf<UInventory_Slot> WidgetClassInventorySlot;

	UPROPERTY(EditAnywhere, Category = "Default")
	UInventory_Slot* W_InventorySlot;
};