#pragma once

#include "CoreMinimal.h"
#include "E_ItemQuality.h"
#include "E_ItemType.h"
#include "Engine/DataTable.h"
#include "ItemDataTable.generated.h"

USTRUCT()
struct FItemDataTable : public FTableRowBase
{
	GENERATED_BODY()

	FItemDataTable();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemQuality Quality;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 PriceValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsSackable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDroppable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* WorldMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration;
};

