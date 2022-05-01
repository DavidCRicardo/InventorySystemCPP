#include "Item/FItemStructure.h"

FItemStructure::FItemStructure()
{
	//ID = FGuid();
	
	ID = FGuid::NewGuid();
	
	Icon = nullptr;
	Name = FText::FromString("None");
	Description = FText::FromString("None");
	ItemType = EItemType::Undefined;
	Quality = EItemQuality::Undefined;
	PriceValue = 0;
	IsStackable = false;
	MaxStackSize = 0;
	Class = nullptr;
	IsDroppable = false;
	SkeletalMesh = nullptr;
	WorldMesh = nullptr;
	Duration = 0.f;
	EquipmentSlot = EEquipmentSlot::Undefined;
	CurrentLevel = 0;
	MaxLevel = 0;
	AmountToAddPerLevel = 0;
	Strength = 0;
	Endurance  = 0;
	Attack = 0;
	Defense = 0;
	Health = 0;
}