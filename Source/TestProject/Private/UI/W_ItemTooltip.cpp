// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_ItemTooltip.h"
#include "MyPlayerController.h"
#include "Components/InventoryComponent.h"
#include "Components/InventoryManagerComponent.h"
#include "Internationalization/StringTableRegistry.h"
#include "Item/FItemType.h"

void UW_ItemTooltip::NativeConstruct()
{

}

void UW_ItemTooltip::InitializeTooltip(const FItemStructure& Item)
{
	SetItemName(Item);

	Icon->SetBrushFromTexture(Item.Icon);

	SetItemType(Item);

	SetDescription(Item);

	SetAttributes(Item);
}

void UW_ItemTooltip::InitializeTooltip2(const FItemStructure& Item, const FSlotStructure& EquippedSlot)
{
	EquippedSlotOnProfile = EquippedSlot;

	SetItemName(Item);

	Icon->SetBrushFromTexture(Item.Icon);

	SetItemType(Item);

	SetDescription(Item);

	SetAttributes(Item);
}

void UW_ItemTooltip::SetDescription(const FItemStructure& Item)
{
	FString LItemDescription = Item.Description.ToString();
	FText ItemDescriptionText = LOCTABLE(COMMON_WORDS, ItemDescription);
	Description->SetText(ItemDescriptionText);
}

void UW_ItemTooltip::SetItemType(const FItemStructure& Item)
{
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
}

void UW_ItemTooltip::SetItemName(const FItemStructure& Item)
{
	FString LItemName = Item.ID.ToString();
	FText ItemNameText = LOCTABLE(COMMON_WORDS, ItemName);
	Name->SetText(ItemNameText);
}

void UW_ItemTooltip::SetAttributes(const FItemStructure& Item)
{
	uint8 InRow = 0;
	uint8 InColumn = 0;

	uint8 TempIndex = 0;
	for (EAttributes Attribute : TEnumRange<EAttributes>())
	{
		UTextBlock* SingleAttribute = NewObject<UTextBlock>();
		UTextBlock* TextBlockTest = NewObject<UTextBlock>();

		FString AttributeString = *UEnum::GetDisplayValueAsText(Attribute).ToString();

		int8 CurrentItemValue = 0;
		int8 FinalValue = 0;
		GetAttributeValueFromItem(Item, Attribute, CurrentItemValue);

		if (CurrentItemValue != 0)
		{
			FString String = AttributeString + ": " + FString::FromInt(CurrentItemValue);
			FText Text = FText::FromString(String);

			SingleAttribute->SetText(Text);
			SingleAttribute->Font.TypefaceFontName = FName(TEXT("Regular"));
			SingleAttribute->Font.Size = 12;

			FText Text2 = FText::FromString("Default String");
			FString StringPositive = " ( + 0 ) ";
			FString StringNegative = " ( - 0 ) ";
			int8 EquippedItemValue = 0;
		
			// There is one equipped Item on that Slot
			if (EquippedSlotOnProfile.Amount > 0)
			{
				GetAttributeValueFromItem(EquippedSlotOnProfile.ItemStructure, Attribute, EquippedItemValue);
				FinalValue = CurrentItemValue - EquippedItemValue;

				if (FinalValue < 0) {
					StringNegative = " ( " + FString::FromInt(FinalValue) + ") ";
					Text2 = FText::FromString(StringNegative);

					TextBlockTest->SetColorAndOpacity(FSlateColor({ 1, 0, 0, 1 }));
				}
				else
				{
					StringPositive = " ( +" + FString::FromInt(FinalValue) + ") ";
					Text2 = FText::FromString(StringPositive);
					
					TextBlockTest->SetColorAndOpacity(FSlateColor({ 0, 1, 0, 1 }));
				}
			}
			else {
				// There is no Equipped Item on that Slot
				FinalValue = CurrentItemValue;
				StringPositive = " ( +" + FString::FromInt(FinalValue) + ") ";
				Text2 = FText::FromString(StringPositive);

				TextBlockTest->SetColorAndOpacity(FSlateColor({ 0, 1, 0, 1 }));
			}

			TextBlockTest->SetText(Text2);
			TextBlockTest->Font.TypefaceFontName = FName(TEXT("Regular"));
			TextBlockTest->Font.Size = 12;
			/**/

			AttributesGrid->AddChildToUniformGrid(SingleAttribute, InRow, InColumn);
			InColumn++;
			AttributesGrid->AddChildToUniformGrid(TextBlockTest, InRow, InColumn);

			InRow++;
			InColumn = 0;
		}

		FFormatNamedArguments Args;
		FText FormattedText;

		Args.Add("Value", CurrentItemValue);

		if (TempIndex == 0)
		{
			FormattedText = FText::Format(
				NSLOCTEXT("MyNamespace", "StrengthKey", "Strength: {Value}"),
				Args
			);
		}
		else if (TempIndex == 1)
		{
			FormattedText = FText::Format(
				NSLOCTEXT("MyNamespace", "EnduranceKey", "Endurance: {Value}"), Args
			);
		}
		else if (TempIndex == 2)
		{
			FormattedText = FText::Format(
				NSLOCTEXT("MyNamespace", "DexterityKey", "Dexterity: {Value}"), Args
			);
		}
		else if (TempIndex == 3)
		{
			FormattedText = FText::Format(
				NSLOCTEXT("MyNamespace", "IntelligenceKey", "Intelligence: {Value}"), Args
			);
		}
		SingleAttribute->SetText(FormattedText);

		TempIndex++;
	}
}

void UW_ItemTooltip::GetAttributeValueFromItem(const FItemStructure& Item, EAttributes Attribute, int8& Value)
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
	}
	else
	{
		Value = 0;
	}
}
