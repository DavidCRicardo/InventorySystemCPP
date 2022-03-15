// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryLayout.h"
#include "Blueprint/UserWidget.h"
#include "MyPlayerController.h"
#include "ProfileLayout.h"
#include "HUDLayout.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UHUDLayout : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UInventoryLayout* InventoryUI;
	
protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
};
