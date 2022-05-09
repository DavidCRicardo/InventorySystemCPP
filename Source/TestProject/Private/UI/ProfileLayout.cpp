// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ProfileLayout.h"
#include "MyHUD.h"
#include "MyPlayerController.h"
#include "Components/TextBlock.h"
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
	// Avoid null slots (doesn't crash, purely design )
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
		CreateAttributesEntry();
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
	RefreshWindow();
}

void UProfileLayout::CreateAttributesEntry()
{
	FWidgetsLayoutBP* WidgetLayout = Cast<AMyHUD>(PlayerController->MyHUD)->GetWidgetBPClass("Attribute_Entry_WBP");
	if (WidgetLayout)
	{
		for (uint8 Index = 0; Index < 4/*(uint8)EAttributes::Count*/; Index++)
		{
			UAttribute_Entry* Entry = CreateWidget<UAttribute_Entry>(this, WidgetLayout->Widget);
			Attributes_ListView->AddItem(Entry);
		}
	}
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
	UpdatePlayerStatsUI();
	/**/
}

void UProfileLayout::InitializePlayerStatsUI2(UUserWidget* Entry2)
{
	UAttribute_Entry* Entry = Cast<UAttribute_Entry>(Entry2);
	
	uint8 Index = 0;
	FText Text;
	uint8 Value;
	
	for (EAttributes Attribute : TEnumRange<EAttributes>())
	{
		FString AttributeString = *UEnum::GetDisplayValueAsText(Attribute).ToString();
		
		if (Attribute == EAttributes::Strength || Attribute == EAttributes::Dexterity ||
			Attribute == EAttributes::Endurance || Attribute == EAttributes::Intelligence)
		{
			FFormatNamedArguments Args;
			
			Value = 0;
		
			FString String = AttributeString + ": " + FString::FromInt(Value);
			Text = FText::FromString(String);

			Args.Add("Value", Value);
	
			FText FormattedText = FText::Format(
				NSLOCTEXT("MyNamespace", "StrengthKey", "Strength: {Value}"),
				Args
			);
			StrengthValue->SetText(FormattedText);
			
			Entry->SetAttributeText(FormattedText);

			Entry->AddToViewport();
			//Attributes_ListView->AddItem(Entry);
			
		}
		Index++;
	}
}


void UProfileLayout::UpdatePlayerStatsUI()
{
	TArray<uint8> Attributes = PlayerController->GetPlayerAttributes();
	
	FFormatNamedArguments Args;
	FText FormattedText;
	uint8 Value;

	for (uint8 Index = 0; Index < (uint8)EAttributes::Count; Index++)
	{
		if(Index == 0)
		{
			/*UWidget* ChildWidget = AttributesBox->GetChildAt(4);
			UTextBlock* TextBlock = Cast<UTextBlock>(ChildWidget);
			if (IsValid(TextBlock))
			{
				FText a = TextBlock->GetText();
				
				TextBlock->SetText(a);
			}*/
			Value = Attributes[Index];
			Args.Add("Value", Value);
	
			FormattedText = FText::Format(
				NSLOCTEXT("MyNamespace", "StrengthKey", "Strength: {Value}"),
				Args
			);
			StrengthValue->SetText(FormattedText);
		}
		else if(Index == 1)
		{
			Value = Attributes[Index];
			Args.Add("Value", Value);
	
			FormattedText = FText::Format(
				NSLOCTEXT("MyNamespace", "EnduranceKey", "Endurance: {Value}"), Args
			);
			EnduranceValue->SetText(FormattedText);
		}
		else if(Index == 2)
		{
			Value = Attributes[Index];
			Args.Add("Value", Value);
	
			FormattedText = FText::Format(
				NSLOCTEXT("MyNamespace", "DexterityKey", "Dexterity: {Value}"), Args
			);
			DexterityValue->SetText(FormattedText);
		}else if(Index == 3)
		{
			Value = Attributes[Index];
			Args.Add("Value", Value);
	
			FormattedText = FText::Format(
				NSLOCTEXT("MyNamespace", "IntelligenceKey", "Intelligence: {Value}"), Args
			);
			IntelligenceValue->SetText(FormattedText);
		}
	}
}

void UProfileLayout::InitializePlayerStatsUI()
{
	uint8 Index = 0;

	FText Text;
	for (EAttributes Attribute : TEnumRange<EAttributes>())
	{
		UUserWidget* Attribute_Entry = CreateWidget(this, UAttribute_Entry::StaticClass());

		
		FString AttributeString = *UEnum::GetDisplayValueAsText(Attribute).ToString();
		
		if (Attribute == EAttributes::Strength || Attribute == EAttributes::Dexterity ||
			Attribute == EAttributes::Endurance || Attribute == EAttributes::Intelligence)
		{
			FFormatNamedArguments Args;
			
			uint8 Value;
			Value = 0;
		
			FString String = AttributeString + ": " + FString::FromInt(Value);
			Text = FText::FromString(String);

			Args.Add("Value", Value);
	
			FText FormattedText = FText::Format(
				NSLOCTEXT("MyNamespace", "StrengthKey", "Strength: {Value}"),
				Args
			);
			StrengthValue->SetText(FormattedText);

			if (UAttribute_Entry* Entry = Cast<UAttribute_Entry>(Attribute_Entry))
			{
				Entry->SetAttributeText(FormattedText);
				Attributes_ListView->AddItem(Entry);

				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Attribute Entry Cast Successful")));
			}else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Attribute Entry Cast Failed")));
			}
			
			/*SingleAttribute->SetText(FormattedText);
			SingleAttribute->Font.TypefaceFontName = FName(TEXT("Regular"));
			SingleAttribute->Font.Size = 12;

			//AttributesBox->AddChild(SingleAttribute);
			AttributesBox->AddChildToVerticalBox(SingleAttribute);*/
		}
		Index++;
	}
}

