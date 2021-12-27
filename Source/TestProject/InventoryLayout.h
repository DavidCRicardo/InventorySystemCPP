// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	
	UPROPERTY()
	class UInventoryComponent* Inventory;
	
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* InventoryGridPanel;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* SortButton;

protected:
	virtual void NativeConstruct() override;

	virtual void OnButtonQuitClicked() override;
	
	void InitializeInventorySlots();

private:
	UPROPERTY(EditAnywhere, Category = "Default")
	TSubclassOf<class UUserWidget> WidgetClassInventory;

	UPROPERTY(EditAnywhere, Category = "Default")
	class UUserWidget* W_InventorySlot;
};