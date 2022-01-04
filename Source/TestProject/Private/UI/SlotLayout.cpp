// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SlotLayout.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

USlotLayout::USlotLayout(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USlotLayout::NativeConstruct()
{
	Super::NativeConstruct();
	
	UE_LOG (LogTemp, Warning, TEXT ("Native InventorySlot!!"));
}

void USlotLayout::InitializeSlot(UTexture2D* BackgroundRef)
{
	TextBlockAmount->SetText(FText::FromString(""));
	Background->SetBrushFromTexture(BackgroundRef);
	Slot_Icon = nullptr;
	
	UE_LOG (LogTemp, Warning, TEXT ("InventorySlot Initialized!!"));
}

void USlotLayout::UpdateSlot(FItemStructure ItemStructure, uint8 Amount)
{
	if (Amount == 0)
	{
		ItemID = FText::FromString("Empty");
		TextBlockAmount->SetText(FText::FromString(""));
		Slot_Icon = nullptr;
	}
	else
	{
		ItemID = ItemStructure.ID;
		TextBlockAmount->SetText(FText::AsNumber(Amount));
		Slot_Icon->SetBrushFromTexture(ItemStructure.Icon);
	}
}

void USlotLayout::SetAmountText(const FText& _newAmount)
{
	TextBlockAmount->SetText(_newAmount);
}

FText USlotLayout::GetAmountText()
{
	return TextBlockAmount->GetText();
}