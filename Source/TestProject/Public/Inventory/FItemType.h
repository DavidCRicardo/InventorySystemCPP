#pragma once

#include "CoreMinimal.h"
#include "FItemType.generated.h"

USTRUCT(BlueprintType)
struct FItemType
{
	GENERATED_BODY();

	static FName Undefined;
	static FName Miscellanious;
	static FName Equipment;
	static FName Consumable;
	static FName Currency;
};