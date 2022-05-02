#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EAttributes : int8
{
	//Undefined = -1	UMETA(DisplayName="Undefined"),
	
	Strength		UMETA(DisplayName="Strength"),
	Endurance		UMETA(DisplayName="Endurance"),
	Dexterity		UMETA(DisplayName="Dexterity"),
	Intelligence	UMETA(DisplayName="Intelligence"),
	Attack			UMETA(DisplayName="Attack"),
	Armor			UMETA(DisplayName="Armor"),
	Health			UMETA(DisplayName="Health"),
	Count			UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EAttributes, EAttributes::Count);