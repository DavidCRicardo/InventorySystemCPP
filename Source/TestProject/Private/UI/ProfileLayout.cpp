// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ProfileLayout.h"
#include "MyHUD.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

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
	
	Super::SetTitleToWindow(FString("PROFILE"));

	CreateChildWidgets();
	InitializeSlots();
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
	
	uint8 NumberOfRows = 2;
	uint8 NumberOfColumns = 2;
	
	FWidgetsLayoutBP* WidgetLayout = Cast<AMyHUD>(PlayerController->MyHUD)->GetWidgetBPClass("SlotLayout_WBP");
	//FWidgetsLayoutBP* WidgetLayout = Cast<AMyHUD>(GetOwningPlayer()->GetHUD())->GetWidgetBPClass("SlotLayout_WBP");
	if (WidgetLayout)
	{
		USlotLayout* W_EquipmentSlot = nullptr;
		UUniformGridSlot* GridSlot = nullptr;
		
		for(int i = 0; i < NumberOfRows; i++)
		{
			for(int j = 0; j < NumberOfColumns; j++)
			{
				W_EquipmentSlot = CreateWidget<USlotLayout>(GetWorld(), WidgetLayout->Widget);

				GridSlot = EquipmentGridPanel->AddChildToUniformGrid(W_EquipmentSlot, i, j);
				GridSlot->SetHorizontalAlignment(HAlign_Center);
				GridSlot->SetVerticalAlignment(VAlign_Center);
				
				EquipmentSlotsArray.Add(W_EquipmentSlot);
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
	
	for (int i = 0; i < (uint8)EEquipmentSlot::Count; i++)
	{
		EquipmentSlotsArray[i]->NativeFromEquipment = true;
		EquipmentSlotsArray[i]->InventorySlotIndex = i;

		if (i == 0)
		{
			EquipmentSlotsArray[i]->UpdateSlot(PlayerController->InventoryComponent->GetItemFromItemDB("No_Weapon"));
		}else if(i == 1)
		{
			EquipmentSlotsArray[i]->UpdateSlot(PlayerController->InventoryComponent->GetItemFromItemDB("No_Chest"));
		}else if(i == 2)
		{
			EquipmentSlotsArray[i]->UpdateSlot(PlayerController->InventoryComponent->GetItemFromItemDB("No_Feet"));
		}else if(i == 3)
		{
			EquipmentSlotsArray[i]->UpdateSlot(PlayerController->InventoryComponent->GetItemFromItemDB("No_Hands"));
		}
	}
}

void UProfileLayout::RefreshWindow()
{
	
}