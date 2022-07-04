#include "Inventory/FSlotStructure.h"

#include "EAttributes.h"

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

uint8 FSlotStructure::GetAttributeValueByAttribute(EAttributes Attribute)
{
	switch (Attribute)
	{
	case EAttributes::Strength:
		return ItemStructure.Strength;
	case EAttributes::Endurance:
		return ItemStructure.Endurance;
	case EAttributes::Dexterity:
		return ItemStructure.Dexterity;
	case EAttributes::Intelligence:
		return ItemStructure.Intelligence;
	default:
		return 0;
	}
}
