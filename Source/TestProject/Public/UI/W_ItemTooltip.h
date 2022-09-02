// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EAttributes.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/VerticalBox.h"
#include "Inventory/FSlotStructure.h"
#include "Inventory/FItemStructure.h"
#include "W_ItemTooltip.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEMCPP_API UW_ItemTooltip : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FSlotStructure EquippedSlotOnProfile;

	UFUNCTION()
	void InitializeTooltip(const FItemStructure& Item);
	UFUNCTION()
	void UpdateTooltipAttributes(const FItemStructure& Item, const FSlotStructure& EquippedSlot);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Index;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Name;
	
	UPROPERTY()
	UImage* Icon;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Description;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Type;
	
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* AttributesGrid;

private:
    virtual void NativeConstruct() override;
	void SetDescription(const FItemStructure& Item);
	void SetItemType(const FItemStructure& Item);
	void SetItemName(const FItemStructure& Item);
	void SetAttributes(const FItemStructure& Item);

	UFUNCTION()
	void GetAttributeValueFromItem(const FItemStructure& Item, EAttributes Attribute, int8& Value);
};