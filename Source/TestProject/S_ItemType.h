#pragma once

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"
#include "S_ItemType.generated.h"

USTRUCT(BlueprintType)
struct FS_ItemType
{
	GENERATED_BODY();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Default")
	FText Undefined;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Default")
	FText Miscellanious;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Default")
	FText Equipment;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Default")
	FText Consumable;
};