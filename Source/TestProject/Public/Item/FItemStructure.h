#pragma once

#include "CoreMinimal.h"
#include "EEquipmentSlot.h"
#include "EItemQuality.h"
#include "EItemType.h"
#include "Engine/DataTable.h"
#include "FItemStructure.generated.h"

USTRUCT()
struct INVENTORYSYSTEMCPP_API FItemStructure : public FTableRowBase
{
	GENERATED_BODY()

	FItemStructure();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid ID;

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
	bool IsStackable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MaxStackSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UClass* Class;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDroppable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SkeletalMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* WorldMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipmentSlot EquipmentSlot;
};

