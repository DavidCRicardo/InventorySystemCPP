// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlotLayout.h"
#include "WindowLayout.h"
#include "Blueprint/UserWidget.h"
#include "ContainerLayout.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEMCPP_API UContainerLayout : public UWindowLayout
{
	GENERATED_BODY()

public:
	UContainerLayout();
	
	virtual void ToggleWindow() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UUniformGridPanel* ContainerGridPanel;
	
	UPROPERTY()
	bool IsStorageContainer;
	
	UPROPERTY()
	TArray<USlotLayout*>ContainerSlotsArray;

	UPROPERTY()
	UTexture2D* Background_Slot;

	UFUNCTION()
	void UpdateContainerSlotsUI(uint8 SlotsPerSow, uint8 NumberOfRows);

private:
	virtual void NativeConstruct() override;
	virtual void InitializeSlots() override;
	virtual void CreateChildWidgets() override;
	virtual void SetIndexToChilds(uint8& IndexStart) override;

	UPROPERTY()
	uint8 InventorySize;
};
