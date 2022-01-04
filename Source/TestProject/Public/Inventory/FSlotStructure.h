// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/FItemStructure.h"
#include "FSlotStructure.generated.h"

USTRUCT(BlueprintType)
struct TESTPROJECT_API FSlotStructure : public FTableRowBase 
{
	GENERATED_BODY();

	FSlotStructure();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Default")
	FItemStructure ItemStructure;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Default")
	uint8 Amount;
};
