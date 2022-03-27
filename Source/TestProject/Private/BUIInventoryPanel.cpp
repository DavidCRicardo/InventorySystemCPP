#include "BUIInventoryPanel.h"
#include "Components/ListView.h"
#include "Inventory/FSlotStructure.h"

void UBUIInventoryPanel::NativeConstruct()
{
	Super::NativeConstruct();

	// Imagine we have an inventory class that provides us with the following:
	TArray<FSlotStructure*> Inventory;
	
	// Tell the list view what to use as the data source when creating widgets
	//InventoryListView->SetListItems(InventoryItems);
}
