#include "Item/FItemStructure.h"

FItemStructure::FItemStructure()
{
	ID = "None";
	Icon = nullptr;
	Name = FText::FromString("None");
	Description = "None";
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
	Dexterity = 0;
	Intelligence = 0;
	Attack = 0;
	Defense = 0;
	Health = 0;
}