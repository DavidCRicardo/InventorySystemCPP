// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_SlotDropDownMenu.h"
#include "Components/Button.h"
#include "MyPlayerController.h"
#include "UI/SlotLayout.h"
#include "UI/HUDLayout.h"
#include "UI/MainLayout.h"
#include "UI/InventoryLayout.h"
#include "UI/ProfileLayout.h"
#include "UI/ContainerLayout.h"

void UW_SlotDropDownMenu::NativeConstruct()
{
	Super::NativeConstruct();

	InitAvailableOptions();

	Btn_UseMultiple->SetIsEnabled(false);
	Btn_Move->SetIsEnabled(false);

	Btn_Use->OnClicked.AddDynamic(this, &UW_SlotDropDownMenu::BtnUseClicked);
	Btn_Move->OnClicked.AddDynamic(this, &UW_SlotDropDownMenu::BtnMoveClicked);
	Btn_Pick->OnClicked.AddDynamic(this, &UW_SlotDropDownMenu::BtnPickClicked);
}

void UW_SlotDropDownMenu::NativeDestruct()
{
	Super::NativeDestruct();

	Btn_Use->OnClicked.RemoveDynamic(this, &UW_SlotDropDownMenu::BtnUseClicked);
	Btn_Move->OnClicked.RemoveDynamic(this, &UW_SlotDropDownMenu::BtnMoveClicked);
	Btn_Pick->OnClicked.RemoveDynamic(this, &UW_SlotDropDownMenu::BtnPickClicked);	
}

void UW_SlotDropDownMenu::InitAvailableOptions() 
{
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetOwningPlayer());

	if (!IsValid(PlayerController))
	{
		return;
	}
	
	if (!IsValid(PlayerController->HUDLayoutReference))
	{
		return;
	}

	USlotLayout* LocalSlot{};
	uint8 Index = PlayerController->MenuAnchorIndex;
	if (PlayerController->MenuAnchorIndex < (uint8)EEquipmentSlot::Count)
	{
		LocalSlot = PlayerController->HUDLayoutReference->MainLayout->Profile->EquipmentSlotsArray[Index];
	}
	else 
	{
		Index = PlayerController->MenuAnchorIndex - (uint8)EEquipmentSlot::Count;
		LocalSlot = PlayerController->HUDLayoutReference->MainLayout->Inventory->InventorySlotsArray[Index];
	}

	if (LocalSlot->IsHovered())
	{
		Btn_Use->SetVisibility(ESlateVisibility::Visible);
		Btn_Pick->SetVisibility(ESlateVisibility::Collapsed);
		Btn_Move->SetVisibility(ESlateVisibility::Visible);
		Btn_UseMultiple->SetVisibility(ESlateVisibility::Visible);
	}
	else 
	{
		Btn_Use->SetVisibility(ESlateVisibility::Collapsed);
		Btn_Pick->SetVisibility(ESlateVisibility::Visible);
		Btn_Move->SetVisibility(ESlateVisibility::Visible);
		Btn_UseMultiple->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UW_SlotDropDownMenu::SetMenuOptions(uint8 LocalNumber)
{
	if (/*NativeFromInventory*/ LocalNumber == 1)
	{
		Btn_Use->SetVisibility(ESlateVisibility::Visible);
		Btn_Pick->SetVisibility(ESlateVisibility::Collapsed);
		Btn_Move->SetVisibility(ESlateVisibility::Visible);
		Btn_UseMultiple->SetVisibility(ESlateVisibility::Visible);
	}
	else if (/*NativeFromContainer*/ LocalNumber == 2)
	{
		Btn_Use->SetVisibility(ESlateVisibility::Collapsed);
		Btn_Pick->SetVisibility(ESlateVisibility::Visible);
		Btn_Move->SetVisibility(ESlateVisibility::Visible);
		Btn_UseMultiple->SetVisibility(ESlateVisibility::Collapsed);
	}
	else 
	{
		Btn_Use->SetVisibility(ESlateVisibility::Visible);
		Btn_Pick->SetVisibility(ESlateVisibility::Visible);
		Btn_Move->SetVisibility(ESlateVisibility::Visible);
		Btn_UseMultiple->SetVisibility(ESlateVisibility::Visible);
	}
}

void UW_SlotDropDownMenu::BtnUseClicked()
{
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetOwningPlayer());
	if (!IsValid(PlayerController))
	{
		return;
	}

	USlotLayout* LocalSlot{};
	uint8 Index = PlayerController->MenuAnchorIndex;
	if (PlayerController->MenuAnchorIndex < (uint8)EEquipmentSlot::Count)
	{
		LocalSlot = PlayerController->HUDLayoutReference->MainLayout->Profile->EquipmentSlotsArray[Index];
	}
	else 
	{
		Index = PlayerController->MenuAnchorIndex - (uint8)EEquipmentSlot::Count;
		LocalSlot = PlayerController->HUDLayoutReference->MainLayout->Inventory->InventorySlotsArray[Index];
	}

	if (LocalSlot)
	{
		LocalSlot->UseItem();
	}
}

void UW_SlotDropDownMenu::BtnPickClicked() 
{
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetOwningPlayer());
	if (!IsValid(PlayerController))
	{
		return;
	}

	USlotLayout* LocalSlot{};
	uint8 Index = PlayerController->MenuAnchorIndex;
	
	LocalSlot = PlayerController->HUDLayoutReference->MainLayout->Container->ContainerSlotsArray[Index];

	if (LocalSlot)
	{
		LocalSlot->UseItem();
	}
}

void UW_SlotDropDownMenu::BtnMoveClicked()
{
	// ...
}
