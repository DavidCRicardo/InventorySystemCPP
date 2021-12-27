// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDLayout.h"
#include "DragWidget.h"

bool UHUDLayout::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UDragWidget* DragWidgetResult = Cast<UDragWidget>(InOperation);
	
	FVector2D DragWindowOffset = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
	FVector2D DragWindowOffsetResult = DragWindowOffset - DragWidgetResult->DragOffset;

	DragWidgetResult->WidgetReference->AddToViewport();
	DragWidgetResult->WidgetReference->SetPositionInViewport(DragWidgetResult->DragOffset, false);
	
	return true;
}
