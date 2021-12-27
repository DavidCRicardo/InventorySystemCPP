#pragma once

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"
#include "S_ItemQuality.generated.h"

USTRUCT(BlueprintType)
struct FS_ItemQuality
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Default")
	FLinearColor Common = FLinearColor(0.3,0.3,0.3,1);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Default")
	FLinearColor UnCommon = FLinearColor(0.3,1,0,1);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Default")
	FLinearColor Rare = FLinearColor(0,0.44,0.87,1);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Default")
	FLinearColor Epic = FLinearColor(0.64, 0.21, 0.93,1);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Default")
	FLinearColor Legendary = FLinearColor(1,0.5,0,1);
};
