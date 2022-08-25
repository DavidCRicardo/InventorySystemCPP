#include "Inventory/FItemStructure.h"

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
	Strength = 0;
	Endurance  = 0;
	Dexterity = 0;
	Intelligence = 0;
	Health = 0;
}

FItemStructure::FItemStructure(FName ID, UTexture2D* Icon, FText Name, FName Description, EItemType ItemType, EItemQuality Quality, uint8 PriceValue,
	bool IsStackable, uint8 MaxStackSize, bool IsDroppable, USkeletalMesh* SkeletalMesh, UStaticMesh* WorldMesh, float Duration, 
	EEquipmentSlot EquipmentSlot, uint8 Strength, uint8 Endurance, uint8 Dexterity, uint8 Intelligence, uint8 Health)
{
	ID = ID;
	Icon = Icon;
	Name = Name;
	Description = Description;
	ItemType = ItemType;
	Quality = Quality;
	PriceValue = PriceValue;
	IsStackable = IsStackable;
	MaxStackSize = MaxStackSize;
	IsDroppable = IsDroppable;
	SkeletalMesh = SkeletalMesh;
	WorldMesh = WorldMesh;
	Duration = Duration;
	EquipmentSlot = EquipmentSlot;
	Strength = Strength;
	Endurance = Endurance;
	Dexterity = Dexterity;
	Intelligence = Intelligence;
	Health = Health;
}