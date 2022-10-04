// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_SlotDropDownMenu.h"
#include "Components/Button.h"
#include "MyPlayerController.h"
#include "UI/SlotLayout.h"
#include "UI/HUDLayout.h"
#include "UI/MainLayout.h"
#include "UI/InventoryLayout.h"
#include "UI/ProfileLayout.h"

void UW_SlotDropDownMenu::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_UseMultiple->SetIsEnabled(false);
	//Btn_Move->SetIsEnabled(false);

	Btn_Use->OnClicked.AddUniqueDynamic(this, &UW_SlotDropDownMenu::BtnUseClicked);
	Btn_Move->OnClicked.AddUniqueDynamic(this, &UW_SlotDropDownMenu::BtnMoveClicked);

	PlayerController = Cast<AMyPlayerController>(GetOwningPlayer());
}

FReply UW_SlotDropDownMenu::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	GEngine->AddOnScreenDebugMessage(9, 1.f, FColor::Turquoise, TEXT("OnMouseButtonDown"));

	return FReply::Handled();
}

void UW_SlotDropDownMenu::BtnUseClicked()
{
	CloseDropDownMenu();

	USlotLayout* LocalSlot{};
	uint8 Index = PlayerController->MenuAnchorIndex;
	if (PlayerController->MenuAnchorIndex < (uint8)EEquipmentSlot::Count)
	{
		LocalSlot = PlayerController->HUDLayoutReference->MainLayout->Profile->EquipmentSlotsArray[Index];
	}
	else {
		Index = PlayerController->MenuAnchorIndex - (uint8)EEquipmentSlot::Count;
		LocalSlot = PlayerController->HUDLayoutReference->MainLayout->Inventory->InventorySlotsArray[Index];
	}

	
	if (LocalSlot)
	{
		LocalSlot->UseItem();
	}
}

void UW_SlotDropDownMenu::BtnMoveClicked()
{
	USlotLayout* LocalSlot{};
	uint8 Index = PlayerController->MenuAnchorIndex;
	if (PlayerController->MenuAnchorIndex < (uint8)EEquipmentSlot::Count)
	{
		LocalSlot = PlayerController->HUDLayoutReference->MainLayout->Profile->EquipmentSlotsArray[Index];
	}
	else {
		Index = PlayerController->MenuAnchorIndex - (uint8)EEquipmentSlot::Count;
		LocalSlot = PlayerController->HUDLayoutReference->MainLayout->Inventory->InventorySlotsArray[Index];
	}

	const FGeometry InGeometry = FGeometry();
	const FPointerEvent InMouseEvent = FPointerEvent();
	UDragDropOperation* OutOperation = nullptr;

	LocalSlot->DragSlot(InGeometry,InMouseEvent, OutOperation);

	CloseDropDownMenu();
}

void UW_SlotDropDownMenu::CloseDropDownMenu() 
{
	USlotLayout* LocalSlot{};
	uint8 Index = PlayerController->MenuAnchorIndex;
	if (PlayerController->MenuAnchorIndex < (uint8)EEquipmentSlot::Count)
	{
		LocalSlot = PlayerController->HUDLayoutReference->MainLayout->Profile->EquipmentSlotsArray[Index];
	}
	else {
		Index = PlayerController->MenuAnchorIndex - (uint8)EEquipmentSlot::Count;
		LocalSlot = PlayerController->HUDLayoutReference->MainLayout->Inventory->InventorySlotsArray[Index];
	}

	LocalSlot->CloseSlotMenu();
	

	if (SlotReference)
	{
		//SlotReference->CloseSlotMenu();
	}
}