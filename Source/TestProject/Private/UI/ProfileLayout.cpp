// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ProfileLayout.h"
#include "MyHUD.h"
#include "MyPlayerController.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

//static const FName COMMON_WORDS = "/Game/UI/Strings/CommonWords.CommonWords";
//static const FName LCOMMON_WORDS = "/Game/UI/COMMON_WORDS.COMMON_WORDS";

UProfileLayout::UProfileLayout()
{
}

void UProfileLayout::ToggleWindow()
{
	// Avoid null slots ( doesn't crash, purely design )
	RefreshWindow(); 
	
	Super::ToggleWindow();
}

void UProfileLayout::NativeConstruct()
{
	Super::NativeConstruct();

	//Outdated - Before Localization
	//Super::SetTitleToWindow(FString("PROFILE"));
	
	//FText Text = NSLOCTEXT("MyNamespace", "PROFILEKey", "PROFILE");
	//If you're using a string table, LOCTABLE for string literals, FromStringTable otherwise.
	//FText Text = FText::FromStringTable(LCOMMON_WORDS, "PROFILEKey");
	FText Text = LOCTABLE(COMMON_WORDS, "PROFILEKey");
	
	Super::SetTitleToWindow(Text);
	
	NumberOfColumns = 2;
	NumberOfRows = 2;

	PlayerController = Cast<AMyPlayerController>(GetOwningPlayer());
	if(IsValid(PlayerController))
	{
		CreateChildWidgets();
    	InitializeSlots();
	}
	
	SetVisibility(ESlateVisibility::Hidden);
}

void UProfileLayout::OnButtonQuitClicked()
{
	if (IsValid(PlayerController))
	{
		PlayerController->ToggleProfile();
	}
}

void UProfileLayout::CreateChildWidgets()
{
	if (!IsValid(PlayerController))
	{
		return;
	}
	
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
	if (!IsValid(PlayerController))
	{
		return;
	}

	RefreshWindow();
}

void UProfileLayout::RefreshWindow()
{
	if (!IsValid(PlayerController) || !IsValid(PlayerController->InventoryManagerComponent))
	{
		return;
	}

	if (!PlayerController->InventoryManagerComponent->Inventory.IsValidIndex(0))
	{
		return;
	}
	
	
	FSlotStructure CurrentSlot = {};
	
	for(int i = 0; i < (uint8)EEquipmentSlot::Count; i++)
	{
		CurrentSlot = PlayerController->InventoryManagerComponent->GetInventorySlot(i);

		/* Update Empty Slot */
		if(CurrentSlot.Amount <= 0){
			FSlotStructure EmptySlot = {};
			//CurrentSlot = PlayerController->InventoryComponent->GetEmptySlot(CurrentSlot.ItemStructure.EquipmentSlot);
			if (i == 0)
			{
				EmptySlot = PlayerController->InventoryManagerComponent->GetItemFromItemDB("No_Weapon");
			}else if(i == 1)
			{
				EmptySlot = PlayerController->InventoryManagerComponent->GetItemFromItemDB("No_Chest");
			}else if(i == 2)
			{
				EmptySlot = PlayerController->InventoryManagerComponent->GetItemFromItemDB("No_Feet");
			}else if(i == 3)
			{
				EmptySlot = PlayerController->InventoryManagerComponent->GetItemFromItemDB("No_Hands");
			}
			CurrentSlot = EmptySlot;
			PlayerController->InventoryManagerComponent->Client_SetInventorySlot(CurrentSlot, i);
		}
	
		EquipmentSlotsArray[i]->UpdateSlot(CurrentSlot);
	}

	// Refresh Stats
	// UI Get Player Stats (target = PC)
	
	FFormatNamedArguments Args;
	
	uint8 Value = 0;
	Value = PlayerController->AttributesMap[EAttributes::Strength];
	Args.Add("Value", Value);
	
	FText FormattedText = FText::Format(
		NSLOCTEXT("MyNamespace", "StrengthKey", "Strength: {Value}"),
		Args
	);
	StrengthValue->SetText(FormattedText);

	
	Value = PlayerController->AttributesMap[EAttributes::Endurance];
	Args.Add("Value", Value);
	
	FormattedText = FText::Format(
		NSLOCTEXT("MyNamespace", "EnduranceKey", "Endurance: {Value}"), Args
	);
	EnduranceValue->SetText(FormattedText);

	
	Value = PlayerController->AttributesMap[EAttributes::Dexterity];
	Args.Add("Value", Value);
	
	FormattedText = FText::Format(
		NSLOCTEXT("MyNamespace", "DexterityKey", "Dexterity: {Value}"), Args
	);
	DexterityValue->SetText(FormattedText);


	Value = PlayerController->AttributesMap[EAttributes::Intelligence];
	Args.Add("Value", Value);
	
	FormattedText = FText::Format(
		NSLOCTEXT("MyNamespace", "IntelligenceKey", "Intelligence: {Value}"), Args
	);
	IntelligenceValue->SetText(FormattedText);
	/**/
	
	/**/
	
	

}