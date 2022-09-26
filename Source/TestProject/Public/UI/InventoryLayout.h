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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UUniformGridPanel* InventoryGridPanel;
	
	UPROPERTY()
	TArray<USlotLayout*>InventorySlotsArray;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Gold_Amount;

	void UpdateGoldAmount();

private:
	virtual void NativeConstruct() override;
};