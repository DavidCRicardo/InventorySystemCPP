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
	
	SlotButton->OnClicked.AddUniqueDynamic(this, &USlotLayout::OnGenerateButtonClicked);
	SlotButton->OnHovered.AddUniqueDynamic(this, &USlotLayout::OnGenerateButtonHovered);
	SlotButton->OnPressed.AddUniqueDynamic(this, &USlotLayout::OnGenerateButtonOnPressed);
	SlotButton->OnReleased.AddUniqueDynamic(this, &USlotLayout::OnGenerateButtonOnReleased);
}

void USlotLayout::OnGenerateButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnGenerateButtonClicked"))
}

void USlotLayout::OnGenerateButtonHovered()
{
	UE_LOG(LogTemp, Warning, TEXT("OnGenerateButtonHovered"))
}
void USlotLayout::OnGenerateButtonOnPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("OnGenerateButtonOnPressed"))
}
void USlotLayout::OnGenerateButtonOnReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("OnGenerateButtonOnReleased"))
}
FReply USlotLayout::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("ButtonDown"))

	return CustomDetectDrag(InMouseEvent, this, EKeys::LeftMouseButton);
	//return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply USlotLayout::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("ButtonUp"))
	
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

void USlotLayout::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UE_LOG(LogTemp, Warning, TEXT("NativeOnDragDetected"))

	//if (ItemID.IsValid())
	if (HasItem())
	{
		UE_LOG(LogTemp, Warning, TEXT("Valid"))


		// Create Widget and set visual drag on DragDropOperation
		
		UDragItem* DragDropOperation = NewObject<UDragItem>();
		DragDropOperation->DraggedSlotInformation = SlotStructure;
		DragDropOperation->DraggedSlotIndex = InventorySlot;
		DragDropOperation->IsDraggedFromInventory = true;

		

	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("NotValid"))
	}
}

bool USlotLayout::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("NativeOnDrop"))

	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void USlotLayout::InitializeSlot(UTexture2D* BackgroundRef)
{
	AmountTextBlock->SetText(FText::FromString(""));
	Background->SetBrushFromTexture(BackgroundRef);
	Icon->SetBrushFromTexture(BackgroundRef);
}

void USlotLayout::UpdateSlot(const FSlotStructure& NewSlotStructure)
{
	SlotStructure = NewSlotStructure;
	if (HasItem())
	{
		//ItemID = SlotStructure.ItemStructure.ID;
		AmountTextBlock->SetText(FText::AsNumber(SlotStructure.Amount));
		Icon->SetBrushFromTexture(SlotStructure.ItemStructure.Icon);
	}
	else
	{
		//ItemID = FGuid();
		AmountTextBlock->SetText(FText::FromString(""));
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
