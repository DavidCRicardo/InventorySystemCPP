// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlotLayout.h"
#include "WindowLayout.h"
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

	UPROPERTY()
	TArray<USlotLayout*>InventorySlotsArray;

protected:
	virtual void NativeConstruct() override;

	virtual void OnButtonQuitClicked() override;
	
	void InitializeInventorySlots();

private:
	UPROPERTY(EditAnywhere, Category = "Default")
	TSubclassOf<USlotLayout> WidgetClassInventorySlot;

	UPROPERTY(EditAnywhere, Category = "Default")
	USlotLayout* W_InventorySlot;
};