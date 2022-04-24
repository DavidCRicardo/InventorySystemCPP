// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractText.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEMCPP_API UInteractText : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KeyToInteract;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractText;

	//UFUNCTION()
	//void ToggleVisibility();
	
protected:
	UFUNCTION()
	virtual void NativeConstruct() override;
};
