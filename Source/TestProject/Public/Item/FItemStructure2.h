#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FItemStructure2.generated.h"

USTRUCT()
struct INVENTORYSYSTEMCPP_API FItemStructure2 : public FTableRowBase
{
	GENERATED_BODY()

	FItemStructure2();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 ID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    uint8 Strength;
        
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    uint8 Endurance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Attack;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Defense;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    uint8 Health;
};

