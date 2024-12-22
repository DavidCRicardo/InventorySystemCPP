// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "W_SlotDropDownMenu.generated.h"

class UButton;
class USlotLayout;

UCLASS()
class INVENTORYSYSTEMCPP_API UW_SlotDropDownMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Use = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Pick = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> Btn_Move = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> Btn_UseMultiple = nullptr;

	// Replace this LocalNumber param with an actual SlotNativeFrom<> Enum
	UFUNCTION()
	void SetMenuOptions(uint8 LocalNumber);

	UFUNCTION()
	void InitAvailableOptions();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void BtnUseClicked();
	UFUNCTION()
	void BtnPickClicked();
	UFUNCTION()
	void BtnMoveClicked();
};
