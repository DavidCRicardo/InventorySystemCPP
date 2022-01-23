#pragma once

#include "CoreMinimal.h"
#include "FSItemType.generated.h"

USTRUCT(BlueprintType)
struct FSItemType
{
	GENERATED_BODY();

	static FText Undefined;
	static FText Miscellanious;
	static FText Equipment;
	static FText Consumable;
};