// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WindowLayout.h"
#include "Blueprint/UserWidget.h"
#include "ContainerLayout.generated.h"

class USlotLayout;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEMCPP_API UContainerLayout : public UWindowLayout
{
	GENERATED_BODY()

public:
	UContainerLayout();
	
	void Function1(ESlateVisibility InVisibility);

	virtual void ToggleWindow() override;
	virtual void OnButtonQuitClicked() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UUniformGridPanel* ContainerGridPanel;
	
	UPROPERTY()
	bool IsStorageContainer;
	
	UPROPERTY()
	TArray<USlotLayout*>ContainerSlotsArray;

private:
	virtual void NativeConstruct() override;
};