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
	virtual void RefreshWindow() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UUniformGridPanel* ContainerGridPanel;

	UPROPERTY()
	bool IsStorageContainer;

	UPROPERTY()
	TArray<UUserWidget*> ContainerSlots;
	UPROPERTY()
	TArray<USlotLayout*>ContainerSlotsArray;

	UPROPERTY()
	UTexture2D* Background_Slot;
private:
	virtual void NativeConstruct() override;
	virtual void InitializeSlots() override;
	virtual void CreateChildWidgets() override;
	virtual void SetIndexToChilds(uint8& IndexStart) override;

	UPROPERTY()
	uint8 NumberOfColumns;
	UPROPERTY()
	uint8 NumberOfRows;
};
