// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "ItemDragVisual.generated.h"

UCLASS()
class INVENTORYSYSTEMCPP_API UItemDragVisual : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category="Drag Item Visual", meta=(BindWidget))
	UImage* Icon;
	UPROPERTY(BlueprintReadOnly, Category="Drag Item Visual", meta=(BindWidget))
	UBorder* ItemBorder;
};
