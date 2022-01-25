#include "UI/ProfileLayout.h"

UProfileLayout::UProfileLayout()
{
	
}

void UProfileLayout::NativeConstruct()
{
	Super::NativeConstruct();
	
	Super::SetTitleToWindow(FString("PROFILE"));

	InitializeSlots();
}

void UProfileLayout::InitializeSlots()
{
	uint8 InventoryIndex = 0;
	for(int i = 0; i < 4; i++)
	{
		/*
		W_InventorySlot = CreateWidget<USlotLayout>(GetWorld(), WidgetClassSlotLayout);
		InventoryGridPanel->AddChildToUniformGrid(W_InventorySlot, i, j);

		W_InventorySlot->InitializeSlot(Background_Slot, GetOwningPlayer());
		W_InventorySlot->InventorySlotIndex = InventoryIndex;
			
		InventorySlotsArray.Add(W_InventorySlot);
		*/

		InventoryIndex++;
	}
}

void UProfileLayout::RefreshWindow()
{
	
}

