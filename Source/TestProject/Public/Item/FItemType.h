#pragma once

#include "CoreMinimal.h"
#include "FItemType.generated.h"

USTRUCT(BlueprintType)
struct FItemType
{
	GENERATED_BODY();

	static FText Undefined;
	static FText Miscellanious;
	static FText Equipment;
	static FText Consumable;
};