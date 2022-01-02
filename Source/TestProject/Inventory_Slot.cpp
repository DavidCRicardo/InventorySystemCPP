// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory_Slot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UInventory_Slot::UInventory_Slot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UInventory_Slot::NativeConstruct()
{
	Super::NativeConstruct();
	
	UE_LOG (LogTemp, Warning, TEXT ("Native InventorySlot!!"));
}

void UInventory_Slot::InitializeSlot(UTexture2D* BackgroundRef)
{
	TextBlockAmount->SetText(FText::FromString(""));
	Background->SetBrushFromTexture(BackgroundRef);
	Slot_Icon = nullptr;
	
	UE_LOG (LogTemp, Warning, TEXT ("InventorySlot Initialized!!"));
}



void UInventory_Slot::UpdateSlot(FItemDataTable ItemData, uint8 Amount)
{
	if (Amount == 0)
	{
		ItemID = FText::FromString("Empty");
		TextBlockAmount->SetText(FText::FromString(""));
		Slot_Icon = nullptr;
	}
	else
	{
		ItemID = ItemData.ID;
		TextBlockAmount->SetText(FText::AsNumber(Amount));
		Slot_Icon->SetBrushFromTexture(ItemData.Icon);
	}
}

void UInventory_Slot::SetAmountText(const FText& _newAmount)
{
	TextBlockAmount->SetText(_newAmount);
}

FText UInventory_Slot::GetAmountText()
{
	return TextBlockAmount->GetText();
}