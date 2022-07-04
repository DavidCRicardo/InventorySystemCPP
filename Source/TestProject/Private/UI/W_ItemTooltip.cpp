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

		uint8 Value;
		GetAttributeValueFromItem(Item, Attribute, Value);

		if (Value != 0)
		{
			FString String = AttributeString + ": " + FString::FromInt(Value);
			FText Text = FText::FromString(String);
		
			SingleAttribute->SetText(Text);
			SingleAttribute->Font.TypefaceFontName = FName(TEXT("Regular"));
			SingleAttribute->Font.Size = 12;

			//VerticalBoxAttributes->AddChild(SingleAttribute);

			/**/
			FString StringPositive = " ( +" + FString::FromInt(Value) + ") ";
			FString StringNegative = " ( -" + FString::FromInt(Value) + ") ";
			FText Text2 = FText::FromString(StringPositive);
			
			TextBlockTest->SetText(Text2);
			TextBlockTest->Font.TypefaceFontName = FName(TEXT("Regular"));
			TextBlockTest->Font.Size = 12;
			TextBlockTest->SetColorAndOpacity(FSlateColor({0,1,0,1}));

			/*if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetOwningPlayer()))
			{
				if (IsValid(PlayerController->InventoryManagerComponent) && IsValid(PlayerController->InventoryManagerComponent->PlayerInventory))
				{
					FSlotStructure LocalSlot = PlayerController->InventoryManagerComponent->PlayerInventory->GetInventorySlot(AttributeIndex);

					for (EAttributes Attribute1 : TEnumRange<EAttributes>())
					{
						uint8 AttributeValue = LocalSlot.GetAttributeValueByAttribute(Attribute1);
					}
				}
			}*/

			AttributesGrid->AddChildToUniformGrid(SingleAttribute, InRow, InColumn);
			InColumn++;
			AttributesGrid->AddChildToUniformGrid(TextBlockTest, InRow, InColumn);

			InRow++;
			InColumn = 0;
			/**/
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
