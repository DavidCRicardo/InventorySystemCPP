// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WindowLayout.h"
#include "InventoryLayout.generated.h"

class USlotLayout;
class UUniformGridPanel;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEMCPP_API UInventoryLayout : public UWindowLayout
{
	GENERATED_BODY()

public:
	UInventoryLayout();

	virtual void ToggleWindow() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UUniformGridPanel* InventoryGridPanel;
	
	UPROPERTY()
	TArray<USlotLayout*>InventorySlotsArray;
	
private:
	virtual void NativeConstruct() override;
};