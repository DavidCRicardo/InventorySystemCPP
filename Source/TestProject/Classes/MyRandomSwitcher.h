// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyRandomSwitcher.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UMyRandomSwitcher : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(meta = (BindWidget))
	class UButton* ButtonForTab1;

    UPROPERTY(meta = (BindWidget))
	class UButton* ButtonForTab2;
	
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* My_WidgetSwitcher;

	UFUNCTION()
    void OnButtonClicked();

	UFUNCTION()
	void OnButtonClicked2();
	
	void NativeConstruct() override;
};
