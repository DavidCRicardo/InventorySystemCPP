// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/FItemStructure.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/FSlotStructure.h"
#include "SlotLayout.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API USlotLayout : public UUserWidget
{
	GENERATED_BODY()

public:
	USlotLayout(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 InventorySlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlotStructure SlotStructure;

	UFUNCTION()
	void UpdateSlot(const FSlotStructure& NewSlotStructure);

	UFUNCTION()
	void InitializeSlot(UTexture2D* BackgroundRef);
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* Icon;
	
	UPROPERTY(meta = (BindWidget))
	class UBorder* SlotBorder;

	UPROPERTY(meta = (BindWidget))
	class UButton* SlotButton;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AmountTextBlock;

	UFUNCTION(BlueprintGetter, BlueprintCallable)
	FText GetAmountText();

	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, Category = "Default")
	TSubclassOf<UTexture2D> WidgetClass;

protected:
	UFUNCTION()
	void OnGenerateButtonClicked();
	UFUNCTION()
	void OnGenerateButtonHovered();
	UFUNCTION()
	void OnGenerateButtonOnPressed();
	UFUNCTION()
	void OnGenerateButtonOnReleased();
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	FReply CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey);


	UFUNCTION()
	bool HasItem();
	
private:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UImage* Background;
};
