// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/FSlotStructure.h"
#include "SlotLayout.generated.h"

DECLARE_LOG_CATEGORY_CLASS(LogSlotLayout, Verbose, Verbose);

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
	uint8 InventorySlotIndex;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlotStructure SlotStructure;

	UFUNCTION()
	void UpdateSlot(const FSlotStructure& NewSlotStructure);
	
	UFUNCTION()
	void InitializeSlot(UTexture2D* BackgroundRef, APlayerController* PlayerControllerReference);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AMyPlayerController* PlayerController;
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* Icon;
	
	UPROPERTY(meta = (BindWidget))
	class UBorder* SlotBorder;

	UPROPERTY(meta = (BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(meta = (BindWidget))
	class UButton* SlotButton;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AmountTextBlock;

	UFUNCTION()
	FLinearColor GetBorderColor();
	
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, Category = "Default")
	TSubclassOf<UTexture2D> WidgetClass;

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	UFUNCTION()
	void ToggleTooltip();
	UFUNCTION()
	void DisplayTooltip();
	UFUNCTION()
	void HideTooltip();
	
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	FReply CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey);
	
	UFUNCTION()
	bool HasItem();
	
private:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UImage* Background;
};