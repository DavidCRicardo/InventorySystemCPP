#pragma once

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"
#include "EItemQuality.generated.h"

UENUM(BlueprintType)
enum class EItemQuality : uint8
{
	Undefined = 0			UMETA(DisplayName="Undefined"),
	Common					UMETA(DisplayName="Common"),
	UnCommon				UMETA(DisplayName="UnCommon"),
	Rare					UMETA(DisplayName="Rare"),
	Epic					UMETA(DisplayName="Epic"),
	Legendary				UMETA(DisplayName="Legendary")
};