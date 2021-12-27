// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryLayout.h"
#include "InventoryComponent.h"
#include "Components/UniformGridPanel.h"

UInventoryLayout::UInventoryLayout()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> InventorySlotObj(TEXT("/Game/UI/BP_InventorySlot"));
	WidgetClassInventory = InventorySlotObj.Class;
}

void UInventoryLayout::NativeConstruct()
{
	Super::NativeConstruct();

	Super::SetTitleToWindow("INVENTORY");
 
	//InitializeInventorySlots();

	UE_LOG (LogTemp, Warning, TEXT ("Inventory Layout Initialized!!"));
}

void UInventoryLayout::InitializeInventorySlots()
{
	UE_LOG (LogTemp, Warning, TEXT ("Initialization started with Inventory Slots!"));
	
	//for(int i = 0; i < Inventory->NumberOfSlots; i++)
	/*for(int i = 0; i < 7; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			W_InventorySlot = CreateWidget<UUserWidget>(GetWorld(), WidgetClassInventory);
			InventoryGridPanel->AddChildToUniformGrid(W_InventorySlot, i, j);
		}
	}*/
	
	UE_LOG (LogTemp, Warning, TEXT ("Initialization terminated with Inventory Slots!"));
}

void UInventoryLayout::OnButtonQuitClicked()
{
	if (IsValid(PlayerController))
	{
		PlayerController->ToggleInventory();
	}
}
