#pragma once

#include "CoreMinimal.h"
#include "EEquipmentSlot.h"
#include "EItemQuality.h"
#include "EItemType.h"
#include "Engine/DataTable.h"
#include "FItemStructure.generated.h"

USTRUCT(BlueprintType)
struct INVENTORYSYSTEMCPP_API FItemStructure : public FTableRowBase
{
	GENERATED_BODY()

	FItemStructure();
	
	/*FItemStructure(FName ID, UTexture2D* Icon, FText Name, FName Description, EItemType ItemType, EItemQuality Quality, uint8 PriceValue, bool IsStackable,
		uint8 MaxStackSize, bool IsDroppable, USkeletalMesh* SkeletalMesh,UStaticMesh* WorldMesh,float Duration,EEquipmentSlot EquipmentSlot,
		uint8 Strength,uint8 Endurance,uint8 Dexterity,uint8 Intelligence,	uint8 Health);*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemQuality Quality;

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
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipmentSlot EquipmentSlot;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    uint8 Strength;
        
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    uint8 Endurance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Dexterity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Intelligence;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    uint8 Health;
};