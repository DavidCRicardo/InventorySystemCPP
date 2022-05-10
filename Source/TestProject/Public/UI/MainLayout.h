// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryLayout.h"
#include "ProfileLayout.h"
#include "Blueprint/UserWidget.h"
#include "MainLayout.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEMCPP_API UMainLayout : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UInventoryLayout* Inventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProfileLayout* Profile;

	UPROPERTY()
	TArray<FSlotStructure> InventorySlots;

protected:
	UFUNCTION()
	virtual void NativeConstruct() override;
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UPROPERTY()
	TArray<USlotLayout*> InventorySlotsLayouts;
};