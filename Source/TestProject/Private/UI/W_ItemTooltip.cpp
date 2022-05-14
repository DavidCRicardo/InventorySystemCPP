// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_ItemTooltip.h"
#include "MyPlayerController.h"
#include "Item/FItemType.h"

void UW_ItemTooltip::NativeConstruct()
{
	
}

void UW_ItemTooltip::InitializeTooltip(const FItemStructure& Item)
{
	// Set Item Name
	FString LItemName = Item.ID.ToString();
	FText ItemNameText = LOCTABLE(COMMON_WORDS, ItemName);
	Name->SetText(ItemNameText);

	// Set Item Icon
	Icon->SetBrushFromTexture(Item.Icon);

	// Set Item Type
	FName LItemType;
	switch (Item.ItemType)
	{
	case EItemType::Consumable:
		LItemType = FItemType::Consumable;
		break;
	case EItemType::Equipment:
		LItemType = FItemType::Equipment;
		break;
	case EItemType::Miscellaneous:
		LItemType = FItemType::Miscellanious;
		break;
	case EItemType::Undefined:
	default:
		LItemType = FItemType::Undefined;
		break;
	}
	FText ItemTypeText = LOCTABLE(COMMON_WORDS, ItemType.ToString());
	Type->SetText(ItemTypeText);

	// Set Description
	FString LItemDescription = Item.Description.ToString();
	FText ItemDescriptionText = LOCTABLE(COMMON_WORDS, ItemDescription);
    Description->SetText(ItemDescriptionText);

	uint8 TempIndex = 0;
	// Set Attributes
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

		FFormatNamedArguments Args;
		FText FormattedText;

		Args.Add("Value", Value);
		
		if(TempIndex == 0)
		{
			FormattedText = FText::Format(
				NSLOCTEXT("MyNamespace", "StrengthKey", "Strength: {Value}"),
				Args
			);
		}
		else if(TempIndex == 1)
		{
			FormattedText = FText::Format(
				NSLOCTEXT("MyNamespace", "EnduranceKey", "Endurance: {Value}"), Args
			);
		}
		else if(TempIndex == 2)
		{
			FormattedText = FText::Format(
				NSLOCTEXT("MyNamespace", "DexterityKey", "Dexterity: {Value}"), Args
			);
		}
		else if(TempIndex == 3)
		{
			FormattedText = FText::Format(
				NSLOCTEXT("MyNamespace", "IntelligenceKey", "Intelligence: {Value}"), Args
			);
		}
		SingleAttribute->SetText(FormattedText);
		
		TempIndex++;
	}	
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
