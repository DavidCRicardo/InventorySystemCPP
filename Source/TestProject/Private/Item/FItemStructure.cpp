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

FItemStructure::FItemStructure(FName ID, UTexture2D* Icon, FText Name, FName Description, EItemType ItemType, EItemQuality Quality, uint8 PriceValue,
	bool IsStackable, uint8 MaxStackSize, bool IsDroppable, USkeletalMesh* SkeletalMesh, UStaticMesh* WorldMesh, float Duration, 
	EEquipmentSlot EquipmentSlot, uint8 CurrentLevel, uint8 MaxLevel, uint8 AmountToAddPerLevel, uint8 Strength, uint8 Endurance, uint8 Dexterity, uint8 Intelligence, 
	uint8 Attack, uint8 Defense, uint8 Health)
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
	CurrentLevel = CurrentLevel;
	MaxLevel = MaxLevel;
	AmountToAddPerLevel = AmountToAddPerLevel;
	Strength = Strength;
	Endurance = Endurance;
	Dexterity = Dexterity;
	Intelligence = Intelligence;
	Attack = Attack;
	Defense = Defense;
	Health = Health;
}