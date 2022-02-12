#pragma once

#include "CoreMinimal.h"
#include "FItemQuality.generated.h"

USTRUCT()
struct FItemQuality
{
	GENERATED_BODY()

	static FLinearColor Undefined;
	static FLinearColor Common;
	static FLinearColor UnCommon;
	static FLinearColor Rare;
	static FLinearColor Epic;
	static FLinearColor Legendary;
};