// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Item/FItemStructure.h"
#include "W_ItemTooltip.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UW_ItemTooltip : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void InitializeTooltip(FItemStructure& Item);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Name;
	
	UPROPERTY()
	UTexture2D* Icon;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Description;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Type;
};