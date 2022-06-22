// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryLayout.h"
#include "UI/SlotLayout.h"
#include "MyHUD.h"
#include "MyPlayerController.h"
#include "Components/UniformGridPanel.h"

UInventoryLayout::UInventoryLayout()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> ObjectFind(TEXT("/Game/UI/Textures/T_UI_Slot"));
	Background_Slot = ObjectFind.Object;
}

void UInventoryLayout::NativeConstruct()
{
	Super::NativeConstruct();

	FText Text = LOCTABLE(COMMON_WORDS, "INVENTORYKey");
	Super::SetTitleToWindow(Text);

	/* TODO: This info needs to be Inventory Manager Component */
	NumberOfRows = 7;
	NumberOfColumns = 4;
	/**/

	if (IsValid(PlayerController))
	{
		InitializeSlots();
		//RefreshWindow();
	}
}

void UInventoryLayout::InitializeSlots()
{
	CreateChildWidgets();
	// uint8 FirstIndex = 0; //(uint8)EEquipmentSlot::Count; // 0 if I want to reset the slot indexes, or Count if I want to keep going
	uint8 FirstIndex = (uint8)EEquipmentSlot::Count;
	SetIndexToChilds(FirstIndex);
}

void UInventoryLayout::CreateChildWidgets()
{
	FWidgetsLayoutBP* WidgetLayout = Cast<AMyHUD>(PlayerController->MyHUD)->GetWidgetBPClass("SlotLayout_WBP");
	if (WidgetLayout)
	{
		USlotLayout* W_InventorySlot = nullptr;
		
		for(int i = 0; i < NumberOfRows; i++)
		{
			for(int j = 0; j < NumberOfColumns; j++)
			{
				W_InventorySlot = CreateWidget<USlotLayout>(GetWorld(), WidgetLayout->Widget);
				InventoryGridPanel->AddChildToUniformGrid(W_InventorySlot, i, j);
			
				InventorySlotsArray.Add(W_InventorySlot);
			}
		}
	}else
	{
		UE_LOG(LogTemp, Error, TEXT("WBP_SlotLayout cannot load class" ))
	}
}

void UInventoryLayout::SetIndexToChilds(uint8& IndexStart)
{
	const FSlotStructure SlotStructure = PlayerController->InventoryManagerComponent->GetEmptySlot(EEquipmentSlot::Undefined);
	
	for(int i = 0; i < InventorySlotsArray.Num(); i++)
	{
		InventorySlotsArray[i]->UpdateSlot(SlotStructure);
		InventorySlotsArray[i]->InventorySlotIndex = IndexStart;
		InventorySlotsArray[i]->NativeFromInventory = true;
		
		IndexStart++;
	}
}

void UInventoryLayout::ToggleWindow()
{
	Super::ToggleWindow();
}

void UInventoryLayout::RefreshWindow()
{
	const uint8 InventoryLimit = PlayerController->InventoryManagerComponent->NumberOfSlots;
	
	FSlotStructure CurrentSlot = {};
	FSlotStructure EmptySlot = {};
	EmptySlot = PlayerController->InventoryManagerComponent->GetEmptySlot(EEquipmentSlot::Undefined);

	for(int i = (uint8)EEquipmentSlot::Count; i < InventoryLimit - 9; i++)
	{
		CurrentSlot = PlayerController->InventoryManagerComponent->GetInventorySlot(i);
		//CurrentSlot = PlayerController->InventoryManagerComponent->PlayerInventory->GetInventorySlot(i);

		/* Update Empty Slot */
		if (CurrentSlot.Amount <= 0)
		{
			PlayerController->InventoryManagerComponent->Client_SetInventorySlotItem(EmptySlot, i);
			CurrentSlot = EmptySlot;
		}

		uint8 CurrentIndex = i - (uint8)EEquipmentSlot::Count;
		InventorySlotsArray[CurrentIndex]->UpdateSlot(CurrentSlot);
	}
}
