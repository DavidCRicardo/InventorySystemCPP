// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryLayout.h"
#include "Internationalization/StringTableRegistry.h"

UInventoryLayout::UInventoryLayout() {}

void UInventoryLayout::NativeConstruct()
{
	Super::NativeConstruct();

	FText Text = LOCTABLE(COMMON_WORDS, "Inventory");
	Super::SetTitleToWindow(Text);
}

void UInventoryLayout::ToggleWindow()
{
	Super::ToggleWindow();
}