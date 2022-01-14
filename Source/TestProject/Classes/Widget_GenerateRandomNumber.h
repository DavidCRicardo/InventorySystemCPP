// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_GenerateRandomNumber.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UWidget_GenerateRandomNumber : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	void GenerateRandom();

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* RandomNumberLabel;

	UPROPERTY(meta = (BindWidget))
		class UButton* GenerateButton;

	UFUNCTION()
	void OnGenerateButtonClicked();

	UFUNCTION()
	void OnGenerateButtonOnReleased();
	UFUNCTION()
	void OnGenerateButtonOnHovered();
	UFUNCTION()
	void OnGenerateButtonOnPressed();
	
	void NativeConstruct() override;
};
