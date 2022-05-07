// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_ItemTooltip.h"

#include "MyPlayerController.h"
#include "Item/FItemType.h"

void UW_ItemTooltip::NativeConstruct()
{
	if (AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwningPlayer()))
	{
		uint8 NumberOfAttributes = PC->InventoryManagerComponent->AttributesArray.Num();

		UTextBlock* TextBlock;
		
		TextAttributeArray.Init(TextBlock, NumberOfAttributes);
	}
}


void UW_ItemTooltip::InitializeTooltip(const FItemStructure& Item)
{
	Name->SetText(Item.Name);
	Icon->SetBrushFromTexture(Item.Icon);

	FText InText;
	switch (Item.ItemType)
	{
	case EItemType::Consumable:
		InText = FItemType::Consumable;
		break;
	case EItemType::Equipment:
		InText = FItemType::Equipment;
		break;
	case EItemType::Miscellaneous:
		InText = FItemType::Miscellanious;
		break;
	case EItemType::Undefined:
	default:
		InText = FItemType::Undefined;
		break;
	}
	
	Type->SetText(InText);

	Description->SetText(Item.Description);

	UTextBlock* SingleAttribute = NewObject<UTextBlock>();
	
	if ( Item.Strength != 0)
	{
		uint8 Value = Item.Strength;
		FText StrengthText = FText::AsNumber(Value);
		
		FString String = "Strength: " + FString::FromInt(Value);
		StrengthText = FText::FromString(String);
		
		//FFormatNamedArguments Args;
		//Args.Add("Value", Value);
		
		/*FText FormattedText = FText::Format(
			NSLOCTEXT("MyNamespace", "StrengthTooltipKey", "Strength: {Value}"),
			Args
		);*/
	
		SingleAttribute->SetText(StrengthText);
		SingleAttribute->Font.TypefaceFontName = FName(TEXT("Regular"));
		SingleAttribute->Font.Size = 12;
		
		VerticalBoxAttributes->AddChild(SingleAttribute);
	
	}/*else
	{
		VerticalBoxAttributes->RemoveChild(SingleAttribute);
	}*/
}