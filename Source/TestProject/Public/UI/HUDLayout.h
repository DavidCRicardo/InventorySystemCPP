// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDLayout.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UHUDLayout : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
};
