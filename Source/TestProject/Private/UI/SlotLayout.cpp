// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SlotLayout.h"
#include "DragItem.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

USlotLayout::USlotLayout(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USlotLayout::NativeConstruct()
{
	Super::NativeConstruct();
	
	/*SlotButton->OnClicked.AddUniqueDynamic(this, &USlotLayout::OnGenerateButtonClicked);
	SlotButton->OnHovered.AddUniqueDynamic(this, &USlotLayout::OnGenerateButtonHovered);
	SlotButton->OnPressed.AddUniqueDynamic(this, &USlotLayout::OnGenerateButtonOnPressed);
	SlotButton->OnReleased.AddUniqueDynamic(this, &USlotLayout::OnGenerateButtonOnReleased);*/
}

void USlotLayout::OnGenerateButtonClicked()
{
	UE_LOG(LogSlotLayout, Warning, TEXT("OnGenerateButtonClicked"))
}

void USlotLayout::OnGenerateButtonHovered()
{
	UE_LOG(LogSlotLayout, Warning, TEXT("OnGenerateButtonHovered"))
}
void USlotLayout::OnGenerateButtonOnPressed()
{
	UE_LOG(LogSlotLayout, Warning, TEXT("OnGenerateButtonOnPressed"))
}
void USlotLayout::OnGenerateButtonOnReleased()
{
	UE_LOG(LogSlotLayout, Warning, TEXT("OnGenerateButtonOnReleased"))
}
FReply USlotLayout::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return CustomDetectDrag(InMouseEvent, this, EKeys::LeftMouseButton);
}

FReply USlotLayout::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogSlotLayout, Warning, TEXT("UP DraggedSlotIndex: %i"), InventorySlotIndex)
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

/* Only Create Drag Visual and initialize a DragDropOperation if Slot has an item and is not empty */
void USlotLayout::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	if (HasItem())
	{
		UDragItem* DragDropOperation = NewObject<UDragItem>();
		DragDropOperation->DefaultDragVisual = this;
		DragDropOperation->Pivot = EDragPivot::CenterCenter;
		
		DragDropOperation->DraggedSlotInformation = SlotStructure;
		DragDropOperation->DraggedSlotIndex = InventorySlotIndex;
		DragDropOperation->IsDraggedFromInventory = true;

		UE_LOG(LogSlotLayout, Warning, TEXT("DraggedSlotIndex: %i"), InventorySlotIndex)
		
		OutOperation = DragDropOperation;
	}
}

/* Called when the Drop occurs in one Slot Layout. If the Slot its dropped outside, so the OnDrop from HUDLayout is called */
bool USlotLayout::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	UE_LOG(LogSlotLayout, Warning, TEXT("This Slot has index: %i"), InventorySlotIndex)
	
	UDragItem* DragDropOperation = Cast<UDragItem>(InOperation);
	if (!IsValid(DragDropOperation))
	{
		return false;
	}
	
	const uint8 LocalDraggedSlot = DragDropOperation->DraggedSlotIndex;

	if (DragDropOperation->IsDraggedFromInventory)
	{
		UE_LOG(LogSlotLayout, Warning, TEXT("InventorySlotIndex: %i , DraggedSlotIndex: %i"), InventorySlotIndex, LocalDraggedSlot)

		PlayerController->MoveInventoryItem(LocalDraggedSlot, InventorySlotIndex);
		
		return true;
	}
	
	return false; 
}

void USlotLayout::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

	UE_LOG(LogSlotLayout, Verbose, TEXT("OnEnter: %i" ),InventorySlotIndex)
}

void USlotLayout::InitializeSlot(UTexture2D* BackgroundRef, APlayerController* PlayerControllerReference)
{
	PlayerController = Cast<AMyPlayerController>(PlayerControllerReference);
	
	AmountTextBlock->SetText(FText::FromString(""));
	Background->SetBrushFromTexture(BackgroundRef);
	Icon->SetRenderOpacity(0.f);
}

void USlotLayout::UpdateSlot(const FSlotStructure& NewSlotStructure)
{
	SlotStructure = NewSlotStructure;
	if (HasItem())
	{
		AmountTextBlock->SetText(FText::AsNumber(SlotStructure.Amount));
		Icon->SetBrushFromTexture(SlotStructure.ItemStructure.Icon);
		Icon->SetRenderOpacity(1.f);
	}
	else
	{
		AmountTextBlock->SetText(FText::FromString(""));
		Icon->SetRenderOpacity(0.f);
	}
}

FText USlotLayout::GetAmountText()
{
	return AmountTextBlock->GetText();
}

bool USlotLayout::HasItem()
{
	return SlotStructure.Amount > 0 ? true : false;
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
