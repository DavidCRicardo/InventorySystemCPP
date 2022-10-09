// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDragVisual.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/FSlotStructure.h"
#include "SlotLayout.generated.h"

class AMyPlayerController;
class UCanvasPanel;
class UTextBlock;
class UButton;
class UMenuAnchor;
class UW_SlotDropDownMenu;
/** Slots used on Inventory, Profile and Containers. 
 *  For slots used on Hotbar, go to Hotbar_Slot
 */
UCLASS()
class INVENTORYSYSTEMCPP_API USlotLayout : public UUserWidget//, public TSharedFromThis<SObjectWidget>
{
	GENERATED_BODY()

public:
	USlotLayout(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
	uint8 InventorySlotIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
	bool NativeFromInventory = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
	bool NativeFromEquipment = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
	bool NativeFromContainer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlotStructure SlotStructure;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContainerSlots")
	bool IsStorageSlot = false;
	
	UFUNCTION()
	void ToggleTooltip();

	UFUNCTION()
	void SetSlotIndex(uint8 Index);
	UFUNCTION()
	void SetSlotStructure(const FSlotStructure& SlotToSet);
	
	UFUNCTION()
	void UpdateSlot(const FSlotStructure& NewSlotStructure);
	UFUNCTION()
	void UpdateSlotInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AMyPlayerController* PlayerController;

	void SetNameBoxVisibility();
	
	UFUNCTION()
	void OpenSlotMenu();
	UFUNCTION()
	void CloseSlotMenu();

	UFUNCTION()
	void UseItem();

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Icon;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* SlotBorder;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(meta = (BindWidget))
	UButton* SlotButton;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmountTextBlock;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* NameBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameText;

	UPROPERTY(meta = (BindWidget))
	UMenuAnchor* SlotMenuAnchor;

protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	FReply CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey);

	UFUNCTION(Category="Default")
	FLinearColor GetBorderColor();
	UFUNCTION(Category="Default")
	void DisplayTooltip();
	UFUNCTION(Category="Default")
	void HideTooltip();
	UFUNCTION(Category="Default")
	bool HasItem();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemDragVisual> ItemDragVisualClass;

	/* Mobile */
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

private:
	UFUNCTION()
	bool IsUnequipping(const uint8& LocalDraggedSlotIndex);
	UFUNCTION()
	bool IsEquipping(const uint8& InventorySlot);

	UPROPERTY()
	class UMyGameInstance* GameInstance;

	bool LeftMouseButtonClickedOnce;
};