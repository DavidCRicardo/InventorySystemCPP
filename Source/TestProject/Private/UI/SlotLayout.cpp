// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SlotLayout.h"
#include "DragItem.h"
#include "MyHUD.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Mouse Button Down")));

	// FEventReply EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	// return EventReply.NativeReply;
	
	return CustomDetectDrag(InMouseEvent, this, EKeys::LeftMouseButton);
}

FReply USlotLayout::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("NativeOnMouseButtonDoubleClick")));

	if (HasItem())
	{
		PlayerController->UI_UseInventoryItem_Implementation(InventorySlotIndex);
	}

	return Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
}

void USlotLayout::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	ItemBorder->SetBrushColor(FItemQuality::Common);

	ToggleTooltip();
}

void USlotLayout::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	ItemBorder->SetBrushColor(GetBorderColor());
}

/* Only Create Drag Visual and initialize a DragDropOperation if Slot has an item and is not empty */
void USlotLayout::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	if (HasItem())
	{
		ItemBorder->SetVisibility(ESlateVisibility::HitTestInvisible);
		
		UDragItem* DragDropOperation = NewObject<UDragItem>();
		DragDropOperation->DefaultDragVisual = this;
		DragDropOperation->Pivot = EDragPivot::MouseDown;
		
		DragDropOperation->DraggedSlotInformation = SlotStructure;
		DragDropOperation->DraggedSlotIndex = InventorySlotIndex;
		DragDropOperation->IsDraggedFromInventory = true;
		
		OutOperation = DragDropOperation;

		HideTooltip();

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("DragDetected Successful")));

	}else
	{
		OutOperation = nullptr;

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Not Available To Drag")));
	}
}

/* Called when the Drop occurs in one Slot Layout. If the Slot its dropped outside, so the OnDrop from HUDLayout is called */
bool USlotLayout::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	UDragItem* DragDropOperation = Cast<UDragItem>(InOperation);
	if (!IsValid(DragDropOperation) || DragDropOperation->DraggedSlotInformation.Amount <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Drag And Drop Operation Is Not Valid")));

		return false;
	}
	
	
	const uint8 LocalDraggedSlot = DragDropOperation->DraggedSlotIndex;

	if (DragDropOperation->IsDraggedFromInventory)
	{
		// Check If Unequipping
		/*if (IsUnequipping(LocalDraggedSlot))
		{
			// Unequip Item From Equipment To Inventory
			// Unequip Item To Inventory
			
			return true;
		}*/

		// Check If Equipping
		/*if (IsEquipping(InventorySlotIndex))
		{
			// Equip Item From Inventory to Equipment
			// Equip Item From Inventory

			return true;
		}*/

		/*MoveItemsInInventory();
		if (IsTryingToSplit())
		{
			Split();
			return true;
		}*/

		// MoveInventoryItem()
		PlayerController->MoveInventoryItem(LocalDraggedSlot, InventorySlotIndex);
		HideTooltip();
		
		return true;
	}else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Not From Inventory? Returned False")));

		// Container Stuff
		return false;
	}

	/*if (DragDropOperation->IsDraggedFromInventory)
	{
		PlayerController->MoveInventoryItem(LocalDraggedSlot, InventorySlotIndex);

		HideTooltip();
		
		return true;
	}*/
	
	return false;
}

void USlotLayout::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("DragCancelled")));

	// DragCancelled called when OnDrop returns false
	
	/* The Slot will stay bugged until the next Refresh() */
	PlayerController->RefreshWidgets();
}

void USlotLayout::InitializeSlot(UTexture2D* BackgroundRef)
{
	Background->SetBrushFromTexture(BackgroundRef);
	AmountTextBlock->SetText(FText::FromString(""));
	ItemBorder->SetBrushColor(GetBorderColor());
}

void USlotLayout::UpdateSlot(const FSlotStructure& NewSlotStructure)
{
	SlotStructure = NewSlotStructure;
	
	if (HasItem())
	{
		AmountTextBlock->SetText(FText::AsNumber(SlotStructure.Amount));
	}
	else
	{
		AmountTextBlock->SetText(FText::FromString(""));
	}
	
	Icon->SetBrushFromTexture(SlotStructure.ItemStructure.Icon);
	ItemBorder->SetBrushColor(GetBorderColor());
	ItemBorder->SetVisibility(ESlateVisibility::Visible);

	ToggleTooltip();
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
	if (HasItem())
	{
		DisplayTooltip();
	}else
	{
		HideTooltip();
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

bool USlotLayout::IsUnequipping(const uint8& LocalDraggedSlotIndex)
{
	const uint8 NumberOfEntries = (uint8)EEquipmentSlot::Count;
	if (LocalDraggedSlotIndex < NumberOfEntries)
	{
		return true;
	}
	return false;
}

bool USlotLayout::IsEquipping(const uint8& InventorySlot)
{
	
	return false;
}
