#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FLootList.generated.h"

USTRUCT(BlueprintType)
struct INVENTORYSYSTEMCPP_API FLootList : public FTableRowBase
{
	GENERATED_BODY()

	FLootList();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsUnique;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MinAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DropChance;
};