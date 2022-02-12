// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryLayout.h"
#include "UI/SlotLayout.h"
#include "MyHUD.h"
#include "Components/UniformGridPanel.h"

UInventoryLayout::UInventoryLayout()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> ObjectFind(TEXT("/Game/UI/Textures/T_UI_Slot"));
	Background_Slot = ObjectFind.Object;
	
	/*static ConstructorHelpers::FObjectFinder<UTexture2D> ObjectItemBorder(TEXT("/Game/Textures/T_UI_Item_Border"));
	DefaultBorder = ObjectItemBorder.Object;*/
}

void UInventoryLayout::NativeConstruct()
{
	Super::NativeConstruct();

	Super::SetTitleToWindow("INVENTORY");

	if (!IsValid(PlayerController))
	{
		return;
	}
	
	InitializeSlots();
	RefreshWindow();
}

void UInventoryLayout::InitializeSlots()
{
	CreateChildWidgets();
	uint8 FirstIndex = 0; //(uint8)EEquipmentSlot::Count;
	SetIndexToChilds(FirstIndex);
}

void UInventoryLayout::CreateChildWidgets()
{
	uint8 NumberOfRows = 7;
	uint8 NumberOfColumns = 4;
	
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
	FSlotStructure SlotStructure = {};
	SlotStructure = PlayerController->InventoryComponent->GetEmptySlot();
	
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
	const uint8 InventoryLimit = PlayerController->InventoryComponent->NumberOfSlots;

	FSlotStructure CurrentSlot = {};
	for(int i = 0; i < InventoryLimit; i++)
	{
		CurrentSlot = PlayerController->InventoryComponent->Inventory[i];

		/* Update Empty Slot */
		if(CurrentSlot.Amount <= 0){
			CurrentSlot = PlayerController->InventoryComponent->GetEmptySlot();
		}
		
		InventorySlotsArray[i]->UpdateSlot(CurrentSlot);
	}
}

void UInventoryLayout::OnButtonQuitClicked()
{
	if (IsValid(PlayerController))
	{
		PlayerController->ToggleInventory();
	}
}