// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DragWidget.h"
#include "MyPlayerController.h"
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
	//UWindowLayout();
	UPROPERTY(EditAnywhere, Category = "Default")
	class TSubclassOf<class UUserWidget> WidgetClass;
	
protected:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WindowTitle;
	
	UPROPERTY(meta = (BindWidget))	
	class UBorder* TopBorder;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AMyPlayerController* PlayerController;
	
	UFUNCTION()
	virtual void OnButtonQuitClicked();
	
	virtual void NativeConstruct() override;
	
	virtual void SetTitleToWindow(FString title = FString("WINDOW"));
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	
	UFUNCTION()
	virtual void MyFunction(FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	
private:
	UDragWidget* DragWidget;
	
	FVector2D DragOffset;
};