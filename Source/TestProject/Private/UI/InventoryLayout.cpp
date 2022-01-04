// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryLayout.h"
#include "UI/SlotLayout.h"
#include "Components/UniformGridPanel.h"
#include "MyPlayerController.h"

UInventoryLayout::UInventoryLayout()
{
	static ConstructorHelpers::FClassFinder<USlotLayout> InventorySlotObj(TEXT("/Game/UI/WBP_InventorySlot"));
	WidgetClassInventorySlot = InventorySlotObj.Class;

	static ConstructorHelpers::FObjectFinder<UTexture2D> ObjectFind(TEXT("/Game/Textures/T_UI_Slot"));
	Background_Slot = ObjectFind.Object;
}

void UInventoryLayout::NativeConstruct()
{
	Super::NativeConstruct();

	Super::SetTitleToWindow("INVENTORY");
 
	InitializeInventorySlots();

	UE_LOG (LogTemp, Warning, TEXT ("Inventory Layout Initialized!!"));
}

void UInventoryLayout::InitializeInventorySlots()
{
	UE_LOG (LogTemp, Warning, TEXT ("Initialization started with Inventory Slots!"));
	
	/*for(int i = 0; i < Inventory->NumberOfSlots; i++)
	{
		uint8 Collumn = i % 7;
		uint8 Row = i / 4;
		
		W_InventorySlot = CreateWidget<UUserWidget>(GetWorld(), WidgetClassInventory);
		InventoryGridPanel->AddChildToUniformGrid(W_InventorySlot, Row, Collumn);
	}*/

	for(int i = 0; i < 7; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			W_InventorySlot = CreateWidget<USlotLayout>(GetWorld(), WidgetClassInventorySlot);
			InventoryGridPanel->AddChildToUniformGrid(W_InventorySlot, i, j);

			W_InventorySlot->InitializeSlot(Background_Slot);

			InventorySlotsArray.Add(W_InventorySlot);
		}
	}
	
	UE_LOG (LogTemp, Warning, TEXT ("Initialization terminated with Inventory Slots!"));
}

void UInventoryLayout::RefreshInventorySlots()
{
	const uint8 InventoryLimit = PlayerController->InventoryComponent->NumberOfSlots;

	/*for(int i = 0; i < InventoryLimit; i++)
	{
		FItemStructure* TempItem = PlayerController->InventoryComponent->Inventory[i];
		if (TempItem != nullptr)
		{
			InventorySlotsArray[i]->UpdateSlot(TempItem->ItemStructure, TempItem->Amount);
		}else
		{
			InventorySlotsArray[i]->UpdateSlot(TempItem->ItemStructure, 0);
		}
	}*/
}

void UInventoryLayout::OnButtonQuitClicked()
{
	if (IsValid(PlayerController))
	{
		PlayerController->ToggleInventory();
	}
}