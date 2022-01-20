// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WindowLayout.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UWindowLayout : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Default")
	TSubclassOf<UUserWidget> WidgetClass;
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WindowTitle;
	
	UPROPERTY(meta = (BindWidget))	
	class UBorder* TopBorder;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AMyPlayerController* PlayerController;
	
	UFUNCTION()
	virtual void OnButtonQuitClicked();
	
	virtual void NativeConstruct() override;
	
	virtual void SetTitleToWindow(FString title = FString("WINDOW"));
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
    	
    FReply CustomDetectDrag(const FPointerEvent& InMouseEvent, UWidget* WidgetDetectingDrag, FKey DragKey);
	
	UFUNCTION()
	virtual void MyFunction(FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	
private:
	UPROPERTY()
	class UDragWidget* DragWidget;
	
	FVector2D DragOffset;
};