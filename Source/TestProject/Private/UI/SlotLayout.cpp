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
	AmountTextBlock->SetText(FText::FromString(""));
	Background->SetBrushFromTexture(BackgroundRef);
	//Icon = nullptr;
	Icon->Brush.TintColor = FSlateColor( {0,0,0,0} );
	
	UE_LOG (LogTemp, Warning, TEXT ("InventorySlot Initialized!!"));
}

void USlotLayout::UpdateSlot(FItemStructure ItemStructure, uint8 Amount)
{
	if (Amount == 0)
	{
		ItemID = FText::FromString("Empty");
		AmountTextBlock->SetText(FText::FromString(""));
		//Icon = nullptr;
		Icon->Brush.TintColor = FSlateColor( {0,0,0,0} );
	}
	else
	{
		ItemID = ItemStructure.ID;
		AmountTextBlock->SetText(FText::AsNumber(Amount));
		Icon->SetBrushFromTexture(ItemStructure.Icon);
		Icon->Brush.TintColor = FSlateColor( {0,0,0,1} );
	}
}

void USlotLayout::SetAmountText(const FText& _newAmount)
{
	AmountTextBlock->SetText(_newAmount);
}

FText USlotLayout::GetAmountText()
{
	return AmountTextBlock->GetText();
}