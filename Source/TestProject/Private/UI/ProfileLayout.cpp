// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ProfileLayout.h"

#include "EAttributes.h"
#include "FWidgetsLayoutBP.h"
#include "MyHUD.h"
#include "MyPlayerController.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Internationalization/StringTableRegistry.h"
#include "UI/Attribute_Entry.h"

UProfileLayout::UProfileLayout() {}

void UProfileLayout::ToggleWindow()
{
	Super::ToggleWindow();
}

void UProfileLayout::NativeConstruct()
{
	Super::NativeConstruct();
	
	FText Text = LOCTABLE(COMMON_WORDS, "PROFILEKey");
	Super::SetTitleToWindow(Text);

	if(IsValid(PlayerController))
	{
		CreateAttributesEntry();

		TArray<uint8> InitAttributes;
		InitAttributes.Init(0, (uint8)EAttributes::Count);
		
		UpdatePlayerStatsUI(InitAttributes);
	}
}

void UProfileLayout::CreateAttributesEntry()
{
	FWidgetsLayoutBP* WidgetLayout = Cast<AMyHUD>(PlayerController->MyHUD)->GetWidgetBPClass("Attribute_Entry_WBP");
	if (WidgetLayout)
	{
		for (EAttributes Attribute : TEnumRange<EAttributes>())
		{
			UAttribute_Entry* Entry = CreateWidget<UAttribute_Entry>(this, WidgetLayout->Widget);
			
			Attributes_ListView->AddItem(Entry);
		}
	}
}

void UProfileLayout::UpdatePlayerStatsUI(const TArray<uint8>& InAttributesArray)
{
	TArray<UObject*> EntriesArray = Attributes_ListView->GetListItems();
	if (EntriesArray.Num() <= 0)
	{
		return;
	}
	
	TArray<uint8> Attributes = InAttributesArray;
	
	FFormatNamedArguments Args;
	FText FormattedText;
	uint8 Value;
	
	for (uint8 Index = 0; Index < (uint8)EAttributes::Count; Index++)
	{
		Value = Attributes[Index];
		Args.Add("Value", Value);
		
		if(Index == 0)
		{
			FormattedText = FText::Format(
				NSLOCTEXT("MyNamespace", "StrengthKey", "Strength: {Value}"),
				Args
			);
		}
		else if(Index == 1)
		{
			FormattedText = FText::Format(
				NSLOCTEXT("MyNamespace", "EnduranceKey", "Endurance: {Value}"), Args
			);
		}
		else if(Index == 2)
		{
			FormattedText = FText::Format(
				NSLOCTEXT("MyNamespace", "DexterityKey", "Dexterity: {Value}"), Args
			);
		}
		else if(Index == 3)
		{
			FormattedText = FText::Format(
				NSLOCTEXT("MyNamespace", "IntelligenceKey", "Intelligence: {Value}"), Args
			);
		}
		
		if (UAttribute_Entry* Entry = Cast<UAttribute_Entry>(EntriesArray[Index]))
		{
			Entry->SetAttributeText(FormattedText);
		}
	}
	Attributes_ListView->RegenerateAllEntries();
}