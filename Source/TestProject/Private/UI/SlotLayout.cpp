// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SlotLayout.h"
#include "DragItem.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/FSItemQuality.h"

USlotLayout::USlotLayout(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USlotLayout::NativeConstruct()
{
	Super::NativeConstruct();
	
	SlotButton->OnHovered.AddUniqueDynamic(this, &USlotLayout::OnGenerateButtonHovered);
	SlotButton->OnUnhovered.AddUniqueDynamic(this, &USlotLayout::OnGenerateButtonOnUnHovered);
}

void USlotLayout::OnGenerateButtonHovered()
{
	UE_LOG(LogSlotLayout, Warning, TEXT("Hovered"))
	ItemBorder->SetBrushColor(FLinearColor {0,0,0,1});
}

void USlotLayout::OnGenerateButtonOnUnHovered()
{
	UE_LOG(LogSlotLayout, Warning, TEXT("UnHovered"))
	ItemBorder->SetBrushColor(GetBorderColor());
}

FReply USlotLayout::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return CustomDetectDrag(InMouseEvent, this, EKeys::LeftMouseButton);
}

/* Only Create Drag Visual and initialize a DragDropOperation if Slot has an item and is not empty */
void USlotLayout::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	if (HasItem())
	{
		//SlotButton->SetVisibility(ESlateVisibility::HitTestInvisible);
		ItemBorder->SetVisibility(ESlateVisibility::HitTestInvisible);
		
		UDragItem* DragDropOperation = NewObject<UDragItem>();
		DragDropOperation->DefaultDragVisual = this;
		DragDropOperation->Pivot = EDragPivot::MouseDown; // TopCenter;
		
		DragDropOperation->DraggedSlotInformation = SlotStructure;
		DragDropOperation->DraggedSlotIndex = InventorySlotIndex;
		DragDropOperation->IsDraggedFromInventory = true;
		
		OutOperation = DragDropOperation;
	}else
	{
		OutOperation = nullptr;
	}
}

/* Called when the Drop occurs in one Slot Layout. If the Slot its dropped outside, so the OnDrop from HUDLayout is called */
bool USlotLayout::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	UDragItem* DragDropOperation = Cast<UDragItem>(InOperation);
	if (!IsValid(DragDropOperation))
	{
		return false;
	}
	
	const uint8 LocalDraggedSlot = DragDropOperation->DraggedSlotIndex;

	if (DragDropOperation->IsDraggedFromInventory)
	{
		PlayerController->MoveInventoryItem(LocalDraggedSlot, InventorySlotIndex);
		
		return true;
	}
	
	return false;
}

void USlotLayout::InitializeSlot(UTexture2D* BackgroundRef, APlayerController* PlayerControllerReference)
{
	PlayerController = Cast<AMyPlayerController>(PlayerControllerReference);
	if (PlayerController)
	{
		Background->SetBrushFromTexture(BackgroundRef);

		AmountTextBlock->SetText(FText::FromString(""));
		ItemBorder->SetBrushColor(GetBorderColor());
	}
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
	
	//SlotButton->SetVisibility(ESlateVisibility::Visible);
	ItemBorder->SetVisibility(ESlateVisibility::Visible);
}

FText USlotLayout::GetAmountText()
{
	return AmountTextBlock->GetText();
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
		return FSItemQuality::Common;
	case EItemQuality::UnCommon:
		return FSItemQuality::UnCommon;
	case EItemQuality::Rare:
		return FSItemQuality::Rare;
	case EItemQuality::Epic:
		return FSItemQuality::Epic;
	case EItemQuality::Legendary:
		return FSItemQuality::Legendary;
	case EItemQuality::Undefined:
	default:
		return FSItemQuality::Undefined;
	}
}

FReply USlotLayout::CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey)
{
	if ( InMouseEvent.GetEffectingButton() == DragKey /*|| PointerEvent.IsTouchEvent()*/ )
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