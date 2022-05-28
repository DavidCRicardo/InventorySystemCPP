// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WindowLayout.h"
#include "Blueprint/UserWidget.h"
#include "ContainerLayout.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEMCPP_API UContainerLayout : public UWindowLayout
{
	GENERATED_BODY()

public:
	UContainerLayout();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UUniformGridPanel* ContainerGridPanel;

private:
	virtual void NativeConstruct() override;
};
