// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemDragVisual.generated.h"

class UBorder;
class UImage;

UCLASS()
class INVENTORYSYSTEMCPP_API UItemDragVisual : public UUserWidget
{
private:
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category="Drag Item Visual", meta=(BindWidget))
	UImage* Icon;
	UPROPERTY(BlueprintReadOnly, Category="Drag Item Visual", meta=(BindWidget))
	UBorder* ItemBorder;
};
