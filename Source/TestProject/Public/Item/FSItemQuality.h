#pragma once

#include "CoreMinimal.h"
#include "FSItemQuality.generated.h"

USTRUCT()
struct FSItemQuality
{
	GENERATED_BODY()

	static FLinearColor Undefined;
	static FLinearColor Common;
	static FLinearColor UnCommon;
	static FLinearColor Rare;
	static FLinearColor Epic;
	static FLinearColor Legendary;
};