#pragma once

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Undefined = 0			UMETA(DisplayName="Undefined"),
	Miscellaneous			UMETA(DisplayName="Miscellaneous"),
	Equipment				UMETA(DisplayName="Equipment"),
	Consumable				UMETA(DisplayName="Consumable")
};


