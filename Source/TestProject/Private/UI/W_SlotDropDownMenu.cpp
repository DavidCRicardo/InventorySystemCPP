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

	//OnNativeVisibilityChanged.AddUFunction(this, "RunThis");
	OnVisibilityChanged.AddUniqueDynamic(this, &UW_SlotDropDownMenu::RunThis2);

	Btn_UseMultiple->SetIsEnabled(false);
	Btn_Move->SetIsEnabled(false);

	Btn_Use->OnClicked.AddUniqueDynamic(this, &UW_SlotDropDownMenu::BtnUseClicked);
	Btn_Move->OnClicked.AddUniqueDynamic(this, &UW_SlotDropDownMenu::BtnMoveClicked);
	Btn_Pick->OnClicked.AddUniqueDynamic(this, &UW_SlotDropDownMenu::BtnPickClicked);

	SetVisibility(ESlateVisibility::Visible);

	PlayerController = Cast<AMyPlayerController>(GetOwningPlayer());
}

void UW_SlotDropDownMenu::RunThis2(ESlateVisibility InVisibility) {


	if (ESlateVisibility::Hidden == InVisibility)
	{
		GEngine->AddOnScreenDebugMessage(12, 1.f, FColor::Blue, TEXT("HIDDEN"));
	}else if (ESlateVisibility::Collapsed == InVisibility)
	{
		GEngine->AddOnScreenDebugMessage(12, 1.f, FColor::Blue, TEXT("COLLAPSE"));
	}
	else if (ESlateVisibility::Visible == InVisibility)
	{
		GEngine->AddOnScreenDebugMessage(12, 1.f, FColor::Blue, TEXT("Visible"));

		RunThis();
	}
	else {
		GEngine->AddOnScreenDebugMessage(12, 1.f, FColor::Blue, TEXT("InVisibility"));
	}
}

void UW_SlotDropDownMenu::RunThis() {

	GEngine->AddOnScreenDebugMessage(11, 1.f, FColor::Blue, TEXT("RUN THIS"));

	if (!PlayerController)
	{
		PlayerController = Cast<AMyPlayerController>(GetOwningPlayer());
		if (!PlayerController)
		{
			return;
		}
	}

	if (!PlayerController->HUDLayoutReference)
	{
		return;
	}

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

	// slot id 0 but it can be native from container
	if (LocalSlot->IsHovered())
	{
		Btn_Use->SetVisibility(ESlateVisibility::Visible);
		Btn_Pick->SetVisibility(ESlateVisibility::Collapsed);
		Btn_Move->SetVisibility(ESlateVisibility::Visible);
		Btn_UseMultiple->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		Btn_Use->SetVisibility(ESlateVisibility::Collapsed);
		Btn_Pick->SetVisibility(ESlateVisibility::Visible);
		Btn_Move->SetVisibility(ESlateVisibility::Visible);
		Btn_UseMultiple->SetVisibility(ESlateVisibility::Collapsed);
	}

	return;

	if (LocalSlot->NativeFromInventory)
	{
		Btn_Use->SetVisibility(ESlateVisibility::Visible);
		Btn_Pick->SetVisibility(ESlateVisibility::Collapsed);
		Btn_Move->SetVisibility(ESlateVisibility::Visible);
		Btn_UseMultiple->SetVisibility(ESlateVisibility::Visible);
	}else if (LocalSlot->NativeFromContainer)
	{
		Btn_Use->SetVisibility(ESlateVisibility::Collapsed);
		Btn_Pick->SetVisibility(ESlateVisibility::Visible);
		Btn_Move->SetVisibility(ESlateVisibility::Visible);
		Btn_UseMultiple->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UW_SlotDropDownMenu::SetMenuOptions(uint8 LocalNumber)
{
	if (NativeFromInventory || LocalNumber == 1)
	{
		Btn_Use->SetVisibility(ESlateVisibility::Visible);
		Btn_Pick->SetVisibility(ESlateVisibility::Collapsed);
		Btn_Move->SetVisibility(ESlateVisibility::Visible);
		Btn_UseMultiple->SetVisibility(ESlateVisibility::Visible);
	}
	else if (NativeFromContainer || LocalNumber == 2)
	{
		Btn_Use->SetVisibility(ESlateVisibility::Collapsed);
		Btn_Pick->SetVisibility(ESlateVisibility::Visible);
		Btn_Move->SetVisibility(ESlateVisibility::Visible);
		Btn_UseMultiple->SetVisibility(ESlateVisibility::Collapsed);
	}
	else {
		Btn_Use->SetVisibility(ESlateVisibility::Visible);
		Btn_Pick->SetVisibility(ESlateVisibility::Visible);
		Btn_Move->SetVisibility(ESlateVisibility::Visible);
		Btn_UseMultiple->SetVisibility(ESlateVisibility::Visible);
	}
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

void UW_SlotDropDownMenu::BtnPickClicked() 
{
	USlotLayout* LocalSlot{};
	uint8 Index = PlayerController->MenuAnchorIndex;
	
	LocalSlot = PlayerController->HUDLayoutReference->MainLayout->Container->ContainerSlotsArray[Index];

	if (LocalSlot)
	{
		LocalSlot->UseItem();
	}

	CloseDropDownMenu();
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

	//LocalSlot->DragSlot(InGeometry,InMouseEvent, OutOperation);

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

	// slot id 0 but it can be native from container
	if (!LocalSlot->IsHovered())
	{
		LocalSlot = PlayerController->HUDLayoutReference->MainLayout->Container->ContainerSlotsArray[Index];
	}

	return;

	//LocalSlot->CloseSlotMenu();
	

	if (SlotReference)
	{
		//SlotReference->CloseSlotMenu();
	}
}