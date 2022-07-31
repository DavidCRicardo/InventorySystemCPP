// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Hotbar_Slot.h"
#include "DragItem.h"
#include "UI/ItemDragVisual.h"
#include "FWidgetsLayoutBP.h"
#include "MyPlayerController.h"
#include "Components/TextBlock.h"
#include "MyHUD.h"

UHotbar_Slot::UHotbar_Slot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	HotbarSlotIndex = 0;
	NativeFromHotbar = true;
}

FReply UHotbar_Slot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return CustomDetectDrag(InMouseEvent, this, EKeys::LeftMouseButton);
}

void UHotbar_Slot::SetKeyNumber(uint8 InNumber)
{
	Number->SetText(FText::AsNumber(InNumber));
}

void UHotbar_Slot::NativeConstruct()
{
	PlayerController = Cast<AMyPlayerController>(GetOwningPlayer());
}

void UHotbar_Slot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (HasItem())
	{
		//HideTooltip();

		FWidgetsLayoutBP* WidgetLayout = Cast<AMyHUD>(PlayerController->HUD_Reference)->GetWidgetBPClass("ItemDragVisual_WBP");
		if (WidgetLayout)
		{ 
			ItemDragVisualClass = WidgetLayout->Widget;
		}

		UItemDragVisual* DragVisual = CreateWidget<UItemDragVisual>(this, ItemDragVisualClass);
		DragVisual->Icon->SetBrushFromTexture(SlotStructure.ItemStructure.Icon);
		DragVisual->ItemBorder->SetBrushColor(ItemBorder->BrushColor);

		UDragItem* DragDropOperation = NewObject<UDragItem>();

		DragDropOperation->DefaultDragVisual = DragVisual;
		DragDropOperation->Pivot = EDragPivot::MouseDown;

		DragDropOperation->DraggedSlotInformation = SlotStructure;
		DragDropOperation->DraggedSlotIndex = HotbarSlotIndex;

		DragDropOperation->IsDraggedFromHotbar = true;


		OutOperation = DragDropOperation;
	}
	else
	{
		OutOperation = nullptr;
	}
}

bool UHotbar_Slot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UDragItem* DragDropOperation = Cast<UDragItem>(InOperation);
	if (!IsValid(DragDropOperation) || DragDropOperation->DraggedSlotInformation.Amount <= 0)
	{
		return false;
	}

	const uint8 DraggedSlotIndex = DragDropOperation->DraggedSlotIndex;
	bool bIsDraggedFromInventory = DragDropOperation->IsDraggedFromInventory;
	bool bIsDraggedFromHotbar = DragDropOperation->IsDraggedFromHotbar;

	IInventoryInterface::Execute_UI_MoveHotbarItem(PlayerController, DraggedSlotIndex, HotbarSlotIndex,
		bIsDraggedFromInventory, bIsDraggedFromHotbar);
	//HideTooltip();

	return true;
}

FReply UHotbar_Slot::CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey)
{
	if (InMouseEvent.GetEffectingButton() == DragKey || InMouseEvent.IsTouchEvent())
	{
		FEventReply Reply;
		Reply.NativeReply = FReply::Handled();

		if (WidgetDetectingDrag)
		{
			TSharedPtr<SWidget> SlateWidgetDetectingDrag = WidgetDetectingDrag->GetCachedWidget();
			if (SlateWidgetDetectingDrag.IsValid())
			{
				Reply.NativeReply = Reply.NativeReply.DetectDrag(SlateWidgetDetectingDrag.ToSharedRef(), DragKey);
				return Reply.NativeReply;
			}
		}
	}

	return FReply::Unhandled();
}

void UHotbar_Slot::UpdateSlot(const FSlotStructure& NewSlotStructure)
{
	SetSlotStructure(NewSlotStructure);

	UpdateSlotInfo();

	//ToggleTooltip();
}

void UHotbar_Slot::SetSlotStructure(const FSlotStructure& SlotToSet) {
	SlotStructure = SlotToSet;
}

void UHotbar_Slot::UpdateSlotInfo()
{
	/*if (!HasItem() || (InventorySlotIndex < (uint8)EEquipmentSlot::Count && NativeFromInventory))
	{
		AmountTextBlock->SetText(FText::FromString(""));
	}
	else
	{
		AmountTextBlock->SetText(FText::AsNumber(SlotStructure.Amount));
	}*/

	Icon->SetBrushFromTexture(SlotStructure.ItemStructure.Icon);
	//ItemBorder->SetBrushColor(GetBorderColor());
}

void UHotbar_Slot::SetSlotIndex(uint8 Index)
{
	HotbarSlotIndex = Index;
}

bool UHotbar_Slot::HasItem()
{
	return SlotStructure.Amount > 0 ? true : false;
}