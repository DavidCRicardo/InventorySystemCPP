// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryLayout.h"
#include "Components/TextBlock.h"
#include "Internationalization/StringTableRegistry.h"
#include "MyPlayerController.h"
#include "MyGameInstance.h"

UInventoryLayout::UInventoryLayout() {}

void UInventoryLayout::NativeConstruct()
{
	Super::NativeConstruct();

	FName InTableID = GameInstance->COMMON_WORDS;
	FText Text = FText::FromStringTable(InTableID, "Inventory");
	Super::SetTitleToWindow(Text);
}

void UInventoryLayout::ToggleWindow()
{
	Super::ToggleWindow();
}

void UInventoryLayout::UpdateGoldAmount() {

	if (AMyPlayerController* Controller = Cast<AMyPlayerController>(GetOwningPlayer()))
	{
		uint8 LocalGoldAmount = Controller->UIGetPlayerGold();

		Gold_Amount->SetText(FText::AsNumber(LocalGoldAmount));
	}
}