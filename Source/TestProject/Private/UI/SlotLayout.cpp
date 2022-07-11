// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SlotLayout.h"
#include "DragItem.h"
#include "FWidgetsLayoutBP.h"
#include "MyHUD.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MyPlayerController.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/FItemQuality.h"
#include "UI/W_ItemTooltip.h"

USlotLayout::USlotLayout(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USlotLayout::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = Cast<AMyPlayerController>(GetOwningPlayer());
}

FReply USlotLayout::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return CustomDetectDrag(InMouseEvent, this, EKeys::LeftMouseButton);
}

FReply USlotLayout::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (NativeFromContainer)
	{
		IInventoryInterface::Execute_UI_UseContainerItem(PlayerController, InventorySlotIndex);
	}
	else
	{
		if (HasItem())
		{
			IInventoryInterface::Execute_UI_UseInventoryItem(PlayerController, InventorySlotIndex);
		}
	}
	
	return Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
}

void USlotLayout::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	
	ItemBorder->SetBrushColor(FItemQuality::Common);

	ToggleTooltip();
}

void USlotLayout::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	
	ItemBorder->SetBrushColor(GetBorderColor());
}

/* Only Create Drag Visual and initialize a DragDropOperation if Slot has an item and is not empty */
void USlotLayout::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	
	if (HasItem())
	{
		ItemBorder->SetVisibility(ESlateVisibility::HitTestInvisible);
		
		UItemDragVisual* DragVisual = CreateWidget<UItemDragVisual>(this, ItemDragVisualClass);
		DragVisual->Icon->SetBrushFromTexture(SlotStructure.ItemStructure.Icon);
		DragVisual->ItemBorder->SetBrushColor(ItemBorder->BrushColor);
		
		UDragItem* DragDropOperation = NewObject<UDragItem>();

		// DragDropOperation->DefaultDragVisual = NewObject<USlotLayout>();
		// DragDropOperation->DefaultDragVisual = this;
		DragDropOperation->DefaultDragVisual = DragVisual;
		DragDropOperation->Pivot = EDragPivot::MouseDown;

		DragDropOperation->DraggedSlotInformation = SlotStructure;
		DragDropOperation->DraggedSlotIndex = InventorySlotIndex;

		if (NativeFromInventory)
		{
			DragDropOperation->IsDraggedFromInventory = true;
		}
		if (NativeFromContainer)
		{
			DragDropOperation->IsDraggedFromContainer = true;
		}
		
		OutOperation = DragDropOperation;

		HideTooltip();
	}else
	{
		OutOperation = nullptr;
	}
}

/* Called when the Drop occurs in one Slot Layout. If the Slot its dropped outside, so the OnDrop from HUDLayout is called */
bool USlotLayout::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	UDragItem* DragDropOperation = Cast<UDragItem>(InOperation);
	if (!IsValid(DragDropOperation) || DragDropOperation->DraggedSlotInformation.Amount <= 0)
	{
		return false;
	}
	
	const uint8 LocalDraggedSlot = DragDropOperation->DraggedSlotIndex;

	// Is Dragged From Inventory
	if (DragDropOperation->IsDraggedFromInventory)
	{
		if (NativeFromContainer)
		{
			// Check If Unequipping
			if (IsUnequipping(LocalDraggedSlot))
			{
				IInventoryInterface::Execute_UI_UnEquipToContainer(PlayerController, LocalDraggedSlot, InventorySlotIndex);
				return true;
			}
			
			IInventoryInterface::Execute_UI_DepositContainerItem(PlayerController, LocalDraggedSlot, InventorySlotIndex);
			return true;
		}

		// Check If Unequipping
		if (IsUnequipping(LocalDraggedSlot))
		{
			IInventoryInterface::Execute_UI_UnEquipInventoryItem(PlayerController, LocalDraggedSlot, InventorySlotIndex);
			return true;
		}

		// Check If Equipping
		if (IsEquipping(InventorySlotIndex))
		{
			IInventoryInterface::Execute_UI_EquipInventoryItem(PlayerController, LocalDraggedSlot, InventorySlotIndex);
			return true;
		}

		bool bSplit = false;
		if (bSplit)
		{
				
		}

		// To Inventory
		IInventoryInterface::Execute_UI_MoveInventoryItem(PlayerController, LocalDraggedSlot, InventorySlotIndex);
		HideTooltip();
		
		return true;
	}
	
	// Is Dragged From Container
	if (DragDropOperation->IsDraggedFromContainer)
	{
		// To Inventory
		if (NativeFromInventory)
		{
			// Are we Equipping
			if (IsEquipping(InventorySlotIndex))
			{
				IInventoryInterface::Execute_UI_EquipFromContainer(PlayerController, LocalDraggedSlot, InventorySlotIndex);
				return true;
			}

			bool bSplit = false;
			if (bSplit)
			{
				// ...
				return true;
			}
			
			IInventoryInterface::Execute_UI_TakeContainerItem(PlayerController, LocalDraggedSlot, InventorySlotIndex);
			return true;
		}

		// To Container
		if (NativeFromContainer)
		{
			bool bSplit = false;
			if (bSplit)
			{
				// ...
				return true;
			}
			
			IInventoryInterface::Execute_UI_MoveContainerItem(PlayerController, LocalDraggedSlot, InventorySlotIndex);
			return true;
		}
	}
	
	return false;
}

void USlotLayout::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("DragCancelled")));
	// DragCancelled called when OnDrop returns false
	
	/* The Slot will stay bugged until the next Refresh() */
	/*UDragItem* DragDropOperation = Cast<UDragItem>(InOperation);
	if (!IsValid(DragDropOperation) || DragDropOperation->DraggedSlotInformation.Amount <= 0)
	{
		return;
	}

	const uint8 LocalDraggedSlot = DragDropOperation->DraggedSlotIndex;
	if (DragDropOperation->IsDraggedFromInventory)
	{
		IInventoryInterface::Execute_UI_MoveInventoryItem(PlayerController, LocalDraggedSlot, InventorySlotIndex);

	}else if (DragDropOperation->IsDraggedFromContainer)
	{
		IInventoryInterface::Execute_UI_MoveContainerItem(PlayerController, LocalDraggedSlot, InventorySlotIndex);
	}*/
}

void USlotLayout::InitializeSlot(UTexture2D* BackgroundRef)
{
	Background->SetBrushFromTexture(BackgroundRef);
	AmountTextBlock->SetText(FText::FromString(""));
	ItemBorder->SetBrushColor(GetBorderColor());
}

/* UpdateSlot needs to be updated and cleaned correctly */
void USlotLayout::UpdateSlot(const FSlotStructure& NewSlotStructure)
{
	SlotStructure = NewSlotStructure;
	
	if (!HasItem() || (InventorySlotIndex < (uint8)EEquipmentSlot::Count && NativeFromInventory))
	{
		AmountTextBlock->SetText(FText::FromString(""));
	}
	else
	{
		AmountTextBlock->SetText(FText::AsNumber(SlotStructure.Amount));
	}

	Icon->SetBrushFromTexture(SlotStructure.ItemStructure.Icon);
	ItemBorder->SetBrushColor(GetBorderColor());
	ItemBorder->SetVisibility(ESlateVisibility::Visible);

	//ToggleTooltip();
}

void USlotLayout::UpdateSlot2()
{
	if (!HasItem() || (InventorySlotIndex < (uint8)EEquipmentSlot::Count && NativeFromInventory))
	{
		AmountTextBlock->SetText(FText::FromString(""));
	}
	else
	{
		AmountTextBlock->SetText(FText::AsNumber(SlotStructure.Amount));
	}

	Icon->SetBrushFromTexture(SlotStructure.ItemStructure.Icon);
	ItemBorder->SetBrushColor(GetBorderColor());
	ItemBorder->SetVisibility(ESlateVisibility::Visible);

	//ToggleTooltip();
}

bool USlotLayout::HasItem()
{
	return SlotStructure.Amount > 0 ? true : false;
}

/* Color Based on the Item Quality */ 
FLinearColor USlotLayout::GetBorderColor()
{
	const EItemQuality ItemQuality = SlotStructure.ItemStructure.Quality;

	switch (ItemQuality)
	{
	case EItemQuality::Common:
		return FItemQuality::Common;
	case EItemQuality::UnCommon:
		return FItemQuality::UnCommon;
	case EItemQuality::Rare:
		return FItemQuality::Rare;
	case EItemQuality::Epic:
		return FItemQuality::Epic;
	case EItemQuality::Legendary:
		return FItemQuality::Legendary;
	case EItemQuality::Undefined:
	default:
		return FItemQuality::Undefined;
	}
}

FReply USlotLayout::CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey)
{
	if ( InMouseEvent.GetEffectingButton() == DragKey || InMouseEvent.IsTouchEvent() )
	{
		FEventReply Reply;
		Reply.NativeReply = FReply::Handled();
		
		if ( WidgetDetectingDrag )
		{
			TSharedPtr<SWidget> SlateWidgetDetectingDrag = WidgetDetectingDrag->GetCachedWidget();
			if ( SlateWidgetDetectingDrag.IsValid() )
			{
				Reply.NativeReply = Reply.NativeReply.DetectDrag(SlateWidgetDetectingDrag.ToSharedRef(), DragKey);
				return Reply.NativeReply;
			}
		}
	}

	return FReply::Unhandled();
}

void USlotLayout::ToggleTooltip()
{
	if (IsValid(ToolTipWidget))
	{
		if (HasItem())
		{
			ToolTipWidget->SetVisibility(ESlateVisibility::Visible);
				//DisplayTooltip();
		}
		else
		{
			ToolTipWidget->SetVisibility(ESlateVisibility::Hidden);
				//HideTooltip();
		}
	}
}

void USlotLayout::DisplayTooltip()
{
	if (ItemBorder->ToolTipWidget)
	{
		return;
	}

	FWidgetsLayoutBP* WidgetLayout = Cast<AMyHUD>(PlayerController->MyHUD)->GetWidgetBPClass("ItemTooltip_WBP");
	if (WidgetLayout)
	{
		UW_ItemTooltip* Tooltip = CreateWidget<UW_ItemTooltip>(GetWorld(), WidgetLayout->Widget);
		Tooltip->InitializeTooltip(SlotStructure.ItemStructure);

		ItemBorder->SetToolTip(Tooltip);
	}
}
void USlotLayout::HideTooltip()
{
	if (ItemBorder->ToolTipWidget)
	{
		ItemBorder->SetToolTip(nullptr);
	}
}

/* Returns true if slot dragged and dropped its from equipment layout */
bool USlotLayout::IsUnequipping(const uint8& LocalDraggedSlotIndex)
{
	const uint8 NumberOfEntries = (uint8)EEquipmentSlot::Count;
	if (LocalDraggedSlotIndex < NumberOfEntries)
	{
		return true;
	}
	return false;
}
/* Returns true if slot that received the drop its from equipment layout */
bool USlotLayout::IsEquipping(const uint8& InventorySlot)
{
	const uint8 NumberOfEntries = (uint8)EEquipmentSlot::Count;
	if (InventorySlot < NumberOfEntries)
	{
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("Cannot equip this"))
	return false;
}