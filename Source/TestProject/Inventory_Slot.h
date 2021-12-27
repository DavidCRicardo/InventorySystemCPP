// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Inventory_Slot.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UInventory_Slot : public UUserWidget
{
	GENERATED_BODY()

public:
	UInventory_Slot(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UImage* Background;

protected:
	
	UPROPERTY(meta = (BindWidget))
	class UImage* Slot_Icon;

	/* If Item Data Icon is valid 
	 * else Default Background */
	UPROPERTY(meta = (BindWidget))
	class UBorder* Slot_Border;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Amount;
	
	UFUNCTION(BlueprintSetter, BlueprintCallable)
	void SetAmountText(const FText& _newAmount);

	UFUNCTION(BlueprintGetter, BlueprintCallable)
	FText GetAmountText();

	UPROPERTY(BlueprintReadOnly)
	AItem* ItemInfo;

	virtual void NativeConstruct() override;

	void InitializeVariables();

	UPROPERTY(EditAnywhere, Category = "Default")
	TSubclassOf<class UTexture2D> WidgetClass;

	UPROPERTY(EditAnywhere, Category = "Default")
	class UTexture2D* Background_Slot;
};
