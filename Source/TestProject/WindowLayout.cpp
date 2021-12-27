// Fill out your copyright notice in the Description page of Project Settings.


#include "WindowLayout.h"

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
	//FVector2D AbsoluteScalar = InMouseEvent.GetScreenSpacePosition();
	//FVector2D DragWindowOffset = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry, AbsoluteScalar);
	DragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

	/*
	FEventReply DetectDragIfPressed_ReturnValue =  UWidgetBlueprintLibrary::DetectDragIfPressed(
		InMouseEvent, this, EKeys::LeftMouseButton);
	*/
	
	FEventReply Reply;
	Reply.NativeReply = FReply::Handled();
	
	TSharedPtr<SWidget> SlateWidgetDetectingDrag = this->GetCachedWidget();
	if ( SlateWidgetDetectingDrag.IsValid() && TopBorder->IsHovered() )
	{
		Reply.NativeReply = Reply.NativeReply.DetectDrag(SlateWidgetDetectingDrag.ToSharedRef(), EKeys::LeftMouseButton);
	}
	
	return Reply.NativeReply;
}

void UWindowLayout::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	
	//UDragDropOperation* DragDropOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	
	UDragWidget* DragDropOperation = NewObject<class UDragWidget>();
	DragDropOperation->DefaultDragVisual = this;
	DragDropOperation->Pivot = EDragPivot::MouseDown;
	
	DragDropOperation->WidgetReference = this;
	DragDropOperation->DragOffset = DragOffset;
	
	this->RemoveFromParent();
	
	OutOperation = DragDropOperation;
}