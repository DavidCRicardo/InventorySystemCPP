// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ProfileLayout.h"
#include "MyHUD.h"
#include "MyPlayerController.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "UI/Attribute_Entry.h"

//static const FName COMMON_WORDS = "/Game/UI/Strings/CommonWords.CommonWords";
//static const FName LCOMMON_WORDS = "/Game/UI/COMMON_WORDS.COMMON_WORDS";

UProfileLayout::UProfileLayout()
{
}

void UProfileLayout::ToggleWindow()
{
	Super::ToggleWindow();
}

void UProfileLayout::NativeConstruct()
{
	Super::NativeConstruct();
	
	//FText Text = NSLOCTEXT("MyNamespace", "PROFILEKey", "PROFILE");
	//If you're using a string table, LOCTABLE for string literals, FromStringTable otherwise.
	//FText Text = FText::FromStringTable(LCOMMON_WORDS, "PROFILEKey");
	FText Text = LOCTABLE(COMMON_WORDS, "PROFILEKey");
	
	Super::SetTitleToWindow(Text);
	
	NumberOfColumns = 2;
	NumberOfRows = 2;

	if(IsValid(PlayerController))
	{
    	InitializeSlots();
		CreateAttributesEntry();

		TArray<uint8> InitAttributes;
		InitAttributes.Init(0, (uint8)EAttributes::Count);
		
		UpdatePlayerStatsUI(InitAttributes);
	}
}

void UProfileLayout::CreateChildWidgets()
{
	FWidgetsLayoutBP* WidgetLayout = Cast<AMyHUD>(PlayerController->MyHUD)->GetWidgetBPClass("SlotLayout_WBP");
	//FWidgetsLayoutBP* WidgetLayout = Cast<AMyHUD>(GetOwningPlayer()->GetHUD())->GetWidgetBPClass("SlotLayout_WBP");
	if (WidgetLayout)
	{
		USlotLayout* W_EquipmentSlot = nullptr;
		uint8 SlotIndex = 0;
		
		for(int i = 0; i < NumberOfRows; i++)
		{
			for(int j = 0; j < NumberOfColumns; j++)
			{
				W_EquipmentSlot = CreateWidget<USlotLayout>(GetWorld(), WidgetLayout->Widget);

				UUniformGridSlot* GridSlot = EquipmentGridPanel->AddChildToUniformGrid(W_EquipmentSlot, i, j);
				GridSlot->SetHorizontalAlignment(HAlign_Center);
				GridSlot->SetVerticalAlignment(VAlign_Center);

				W_EquipmentSlot->InventorySlotIndex = SlotIndex;
				EquipmentSlotsArray.Add(W_EquipmentSlot);

				SlotIndex++;
			}
		}
	}
}

void UProfileLayout::InitializeSlots()
{
	CreateChildWidgets();
	uint8 FirstIndex = 0;
	SetIndexToChilds(FirstIndex);
}

void UProfileLayout::SetIndexToChilds(uint8& IndexStart)
{
	//const FSlotStructure SlotStructure = PlayerController->InventoryManagerComponent->GetEmptySlot(EEquipmentSlot::Undefined);
	FSlotStructure EmptySlot{};

	for (int i = 0; i < EquipmentSlotsArray.Num(); i++)
	{
		if (i == 0)
		{
			EmptySlot = PlayerController->InventoryManagerComponent->GetItemFromItemDB("No_Weapon");
		}
		else if (i == 1)
		{
			EmptySlot = PlayerController->InventoryManagerComponent->GetItemFromItemDB("No_Chest");
		}
		else if (i == 2)
		{
			EmptySlot = PlayerController->InventoryManagerComponent->GetItemFromItemDB("No_Feet");
		}
		else if (i == 3)
		{
			EmptySlot = PlayerController->InventoryManagerComponent->GetItemFromItemDB("No_Hands");
		}

		EquipmentSlotsArray[i]->UpdateSlot(EmptySlot);
		EquipmentSlotsArray[i]->InventorySlotIndex = IndexStart;
		EquipmentSlotsArray[i]->NativeFromInventory = true;

		IndexStart++;
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