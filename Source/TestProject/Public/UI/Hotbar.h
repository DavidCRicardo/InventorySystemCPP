// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Hotbar.generated.h"

class USlotLayout;
class UUniformGridPanel;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEMCPP_API UHotbar : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UUniformGridPanel* HotbarGridPanel;

	UPROPERTY()
	TArray<USlotLayout*>HotbarSlotsArray;
};