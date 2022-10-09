// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_SlotDropDownMenu.generated.h"

/**
 * 
 */
class AMyPlayerController;
class UButton;
class USlotLayout;
UCLASS()
class INVENTORYSYSTEMCPP_API UW_SlotDropDownMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UButton* Btn_Use;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* Btn_Pick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* Btn_Move;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* Btn_UseMultiple;

	UPROPERTY(EditAnywhere)
	USlotLayout* SlotReference;
	UPROPERTY(EditAnywhere)
	uint8 ThisIsANumber = 0;

	UPROPERTY(EditAnywhere)
	bool NativeFromInventory;

	UPROPERTY(EditAnywhere)
	bool NativeFromContainer;

	UFUNCTION()
	void SetMenuOptions(uint8 LocalNumber);

	UFUNCTION()
	void RunThis();
	UFUNCTION()
	void RunThis2(ESlateVisibility InVisibility);

protected:
	UFUNCTION()
	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION()
	void BtnUseClicked();
	UFUNCTION()
	void BtnPickClicked();
	UFUNCTION()
	void BtnMoveClicked();

	UFUNCTION()
	void CloseDropDownMenu();

	UPROPERTY()
	AMyPlayerController* PlayerController;
};
