#include "BUIInventoryEntry.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Inventory/FSlotStructure.h"

void UBUIInventoryEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	FSlotStructure* Item = Cast<FSlotStructure>(ListItemObject);
	NameLabel->SetText(Item->ItemStructure.Name);
	IconImage->SetBrushFromTexture(Item->ItemStructure.Icon);
}
