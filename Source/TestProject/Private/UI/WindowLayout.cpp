// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WindowLayout.h"
#include "DragWidget.h"
#include "MyPlayerController.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UWindowLayout::NativeConstruct()
{
	Super::NativeConstruct();

	QuitButton->OnClicked.AddUniqueDynamic(this, &UWindowLayout::OnButtonQuitClicked);
	
	PlayerController = Cast<AMyPlayerController>(GetOwningPlayer());

	SetTitleToWindow();

	TopBorder->OnMouseButtonDownEvent.BindUFunction(this,"MyFunction");
}

void UWindowLayout::MyFunction(FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//UE_LOG (LogTemp, Warning, TEXT ("Border1 - Mouse Button Down Event!"))
}

void UWindowLayout::SetTitleToWindow(FString title)
{
	WindowTitle->SetText(FText::FromString(title));
}

void UWindowLayout::OnButtonQuitClicked()
{
	
}

FReply UWindowLayout::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	/*FEventReply ReplyResult =
		UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	return ReplyResult.NativeReply;*/

	return CustomDetectDrag(InMouseEvent, this, EKeys::LeftMouseButton);
}

void UWindowLayout::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UDragWidget* DragDropOperation = NewObject<UDragWidget>();
	this->SetVisibility(ESlateVisibility::HitTestInvisible);
	
	DragDropOperation->WidgetReference = this;
	DragDropOperation->DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());;

	DragDropOperation->DefaultDragVisual = this;
	DragDropOperation->Pivot = EDragPivot::MouseDown;
	
	OutOperation = DragDropOperation;
}

void UWindowLayout::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	RemoveFromParent();
}

FReply UWindowLayout::CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey)
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

