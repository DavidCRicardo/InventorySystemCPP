// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ContainerLayout.h"

#include "MyHUD.h"
#include "Components/UniformGridPanel.h"
#include "UI/SlotLayout.h"

UContainerLayout::UContainerLayout()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> ObjectFind(TEXT("/Game/UI/Textures/T_UI_Slot"));
	Background_Slot = ObjectFind.Object;
	
	IsStorageContainer = false;
}

void UContainerLayout::ToggleWindow()
{
	Super::ToggleWindow();
}

void UContainerLayout::NativeConstruct()
{
	Super::NativeConstruct();

	FText Text = LOCTABLE(COMMON_WORDS, "CONTAINERKey");
	Super::SetTitleToWindow(Text);
	
	/* TODO: This info needs to be Inventory Manager Component */
	NumberOfRows = 3;
	NumberOfColumns = 3;
	/**/
	
	if (IsValid(PlayerController))
	{
		InitializeSlots();
		RefreshWindow();
	}
}

void UContainerLayout::InitializeSlots()
{
	CreateChildWidgets();
	uint8 FirstIndex = 28 + 4 ;
	SetIndexToChilds(FirstIndex);
}

void UContainerLayout::CreateChildWidgets()
{
	FWidgetsLayoutBP* WidgetLayout = Cast<AMyHUD>(PlayerController->MyHUD)->GetWidgetBPClass("SlotLayout_WBP");
	if (WidgetLayout)
	{
		USlotLayout* W_ContainerSlot = nullptr;
		
		for(int i = 0; i < NumberOfRows; i++)
		{
			for(int j = 0; j < NumberOfColumns; j++)
			{
				W_ContainerSlot = CreateWidget<USlotLayout>(GetWorld(), WidgetLayout->Widget);
				ContainerGridPanel->AddChildToUniformGrid(W_ContainerSlot, i, j);
			
				ContainerSlotsArray.Add(W_ContainerSlot);
			}
		}
	}
}
void UContainerLayout::SetIndexToChilds(uint8& IndexStart)
{
	const FSlotStructure SlotStructure = PlayerController->InventoryManagerComponent->GetEmptySlot(EEquipmentSlot::Undefined);
	
	for(int i = 0; i < ContainerSlotsArray.Num(); i++)
	{
		ContainerSlotsArray[i]->UpdateSlot(SlotStructure);
		ContainerSlotsArray[i]->InventorySlotIndex = IndexStart;
		ContainerSlotsArray[i]->NativeFromContainer = true;
		
		ContainerSlotsArray[i]->IsStorageSlot = true;
		
		IndexStart++;
	}
}

void UContainerLayout::RefreshWindow()
{
	//const uint8 InventoryLimit = PlayerController->InventoryManagerComponent->NumberOfSlots;
	const uint8 InventoryLimit = PlayerController->InventoryManagerComponent->PlayerInventory->Inventory.Num();

	FSlotStructure CurrentSlot = {};
	FSlotStructure EmptySlot = {};
	EmptySlot = PlayerController->InventoryManagerComponent->GetEmptySlot(EEquipmentSlot::Undefined);

	for(int i = 32; i < InventoryLimit; i++)
	{
		//CurrentSlot = PlayerController->InventoryManagerComponent->GetInventorySlot(i);
		CurrentSlot = PlayerController->InventoryManagerComponent->PlayerInventory->GetInventorySlot(i);

		/* Update Empty Slot */
		if(CurrentSlot.Amount <= 0){
			PlayerController->InventoryManagerComponent->Client_SetInventorySlot(EmptySlot, i);
			CurrentSlot = EmptySlot;
		}

		uint8 CurrentIndex = i - (uint8)EEquipmentSlot::Count - 28;
		ContainerSlotsArray[CurrentIndex]->UpdateSlot(CurrentSlot);
	}
}