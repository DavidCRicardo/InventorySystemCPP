// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Attribute_Entry.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEMCPP_API UAttribute_Entry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetAttributeText(FText& Text);

	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AttributeTextBlock;

private:
	virtual void NativeConstruct() override;
	
};