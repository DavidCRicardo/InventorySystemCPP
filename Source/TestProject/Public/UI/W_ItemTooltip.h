// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EAttributes.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Item/FItemStructure.h"
#include "W_ItemTooltip.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEMCPP_API UW_ItemTooltip : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void InitializeTooltip(const FItemStructure& Item);

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
	UVerticalBox* VerticalBoxAttributes;

private:
    virtual void NativeConstruct() override;

	UFUNCTION()
	void GetAttributeValueFromItem(const FItemStructure& Item, EAttributes Attribute, uint8& Value);
};