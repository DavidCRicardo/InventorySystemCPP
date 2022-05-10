// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_ItemTooltip.h"

#include "MyPlayerController.h"
#include "Item/FItemType.h"

void UW_ItemTooltip::NativeConstruct()
{
	/*if (AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwningPlayer()))
	{
		const uint8 NumberOfAttributes = PC->InventoryManagerComponent->AttributesArray.Num();

		UTextBlock* TextBlock;
		TextAttributeArray.Init(TextBlock, NumberOfAttributes);
	}*/
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

	for (EAttributes Attribute : TEnumRange<EAttributes>())
	{
		UTextBlock* SingleAttribute = NewObject<UTextBlock>();
		FString AttributeString = *UEnum::GetDisplayValueAsText(Attribute).ToString();

		uint8 Value;
		GetAttributeValueFromItem(Item, Attribute, Value);

		if (Value != 0)
		{
			FString String = AttributeString + ": " + FString::FromInt(Value);
			FText Text = FText::FromString(String);
		
			SingleAttribute->SetText(Text);
			SingleAttribute->Font.TypefaceFontName = FName(TEXT("Regular"));
			SingleAttribute->Font.Size = 12;
		
			VerticalBoxAttributes->AddChild(SingleAttribute);
			//VerticalBoxAttributes->AddChildToVerticalBox(SingleAttribute);
		}
	}

		/*Localization to be implemented on the next version*/
		//FFormatNamedArguments Args;
		//Args.Add("Value", Value);
		
		//FText FormattedText = FText::Format(
		//	NSLOCTEXT("MyNamespace", "StrengthTooltipKey", "Strength: {Value}"),
		//	Args
		//);
	//}*/
}

void UW_ItemTooltip::GetAttributeValueFromItem(const FItemStructure& Item, EAttributes Attribute, uint8& Value)
{
	if (Attribute == EAttributes::Strength)
	{
		Value = Item.Strength;
	}
	else if (Attribute == EAttributes::Endurance)
	{
		Value = Item.Endurance;
	}
	else if (Attribute == EAttributes::Dexterity)
	{
		Value = Item.Dexterity;
	}
	else if (Attribute == EAttributes::Intelligence)
	{
		Value = Item.Intelligence;
	}else
	{
		Value = 0;
	}
}