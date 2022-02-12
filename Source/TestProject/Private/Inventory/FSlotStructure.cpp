#include "Inventory/FSlotStructure.h"

FSlotStructure::FSlotStructure()
{
	ItemStructure = {};
	Amount = 0;
}

void FSlotStructure::InitSlot(const FItemStructure& InItemStructure, const uint8& InAmount)
{
	ItemStructure = InItemStructure;
	Amount = InAmount;
}