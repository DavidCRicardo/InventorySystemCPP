// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainLayout.h"

#include "DragItem.h"
#include "DragWidget.h"
#include "MyPlayerController.h"
#include "UI/Attribute_Entry.h"
#include "UI/InventoryLayout.h"
#include "UI/ProfileLayout.h"
#include "UI/ItemDragVisual.h"

void UMainLayout::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

bool UMainLayout::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	UDragWidget* DragWidgetResult = Cast<UDragWidget>(InOperation);
	if (!IsValid(DragWidgetResult))
	{
		// Drop Dragged Inventory Item
		UDragItem* DragItemResult = Cast<UDragItem>(InOperation);
		if (!IsValid(DragItemResult))
		{
			return false;
		}

		if (DragItemResult->IsDraggedFromInventory)
		{
			if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetOwningPlayer()))
			{
				IInventoryHUDInterface::Execute_UI_DropInventoryItem(PlayerController, DragItemResult->DraggedSlotIndex);
				return true;
			}
		}
		
		return false;
	} //Drop Dragged UI Window
	else
	{
		const FVector2D DragWindowOffset = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
		const FVector2D DragWindowOffsetResult = DragWindowOffset - DragWidgetResult->DragOffset;

		DragWidgetResult->WidgetReference->AddToViewport();
		DragWidgetResult->WidgetReference->SetVisibility(ESlateVisibility::Visible);
		DragWidgetResult->WidgetReference->SetPositionInViewport(DragWindowOffsetResult, false);

		if (Inventory->GetVisibility() == ESlateVisibility::HitTestInvisible)
		{
			Inventory->SetVisibility(ESlateVisibility::Visible);
			
		}
		if (Profile->GetVisibility() == ESlateVisibility::HitTestInvisible)
		{
			Profile->SetVisibility(ESlateVisibility::Visible);
		}
		
		return true;
	}
}

FReply UMainLayout::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//if (Inventory)
	//{
	//	if (Inventory->DraggedSlot)
	//	{
	//		UItemDragVisual* LocalVisual = Inventory->DraggedSlot;
	//		//LocalVisual->SetPositionInViewport(InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()));
	//		//LocalVisual->SetPositionInViewport(InMouseEvent.GetScreenSpacePosition()); // better but not good

	//		LocalVisual->SetPositionInViewport(InMouseEvent.GetLastScreenSpacePosition());
	//	}
	//}
	
	return FReply::Handled();
}