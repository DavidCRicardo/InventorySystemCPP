#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EEquipmentSlot : int8
{
	Undefined = -1		UMETA(DisplayName="Undefined"),
	Weapon				UMETA(DisplayName="Weapon"),
	Chest				UMETA(DisplayName="Chest"),
	Feet				UMETA(DisplayName="Feet"),
	Hands				UMETA(DisplayName="Hands"),
	Legs				UMETA(DisplayName="Legs"),
	Head				UMETA(DisplayName="Head"),
	Count				UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EEquipmentSlot, EEquipmentSlot::Count);