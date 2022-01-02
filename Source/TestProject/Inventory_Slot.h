// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Blueprint/UserWidget.h"
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemID;

	UFUNCTION()
	void UpdateSlot(FItemDataTable ItemData, uint8 Amount);

	UFUNCTION()
	void InitializeSlot(UTexture2D* BackgroundRef);
	
protected:

	
	UPROPERTY(meta = (BindWidget))
	class UImage* Slot_Icon;

	/* If Item Data Icon is valid 
	 * else Default Background */
	UPROPERTY(meta = (BindWidget))
	class UBorder* Slot_Border;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlockAmount;
	
	UFUNCTION(BlueprintSetter, BlueprintCallable)
	void SetAmountText(const FText& _newAmount);

	UFUNCTION(BlueprintGetter, BlueprintCallable)
	FText GetAmountText();

	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, Category = "Default")
	TSubclassOf<class UTexture2D> WidgetClass;

	UPROPERTY(EditAnywhere, Category = "Default")
	UTexture2D* Background_Slot;

private:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UImage* Background;
};
