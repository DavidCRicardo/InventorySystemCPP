// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_SlotDropDownMenu.generated.h"

/**
 * 
 */
class UButton;
UCLASS()
class INVENTORYSYSTEMCPP_API UW_SlotDropDownMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//UW_SlotDropDownMenu();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UButton* Btn_Use;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* Btn_Move;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* Btn_UseMultiple;

protected:
	UFUNCTION()
	virtual void NativeConstruct() override;

	UFUNCTION()
	void BtnUseClicked();
	UFUNCTION()
	void BtnMoveClicked();
};
