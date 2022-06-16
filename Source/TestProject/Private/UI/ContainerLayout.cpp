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
	
	if (IsValid(PlayerController))
	{
		//InitializeSlots();
		//RefreshWindow();
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
		
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
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
	//const uint8 InventoryLimit = PlayerController->InventoryManagerComponent->ContainerInventory->Inventory.Num();

	FSlotStructure CurrentSlot = {};
	FSlotStructure EmptySlot = {};
	EmptySlot = PlayerController->InventoryManagerComponent->GetEmptySlot(EEquipmentSlot::Undefined);

	for(int i = 0; i <  ContainerSlotsArray.Num(); i++)
	{
		CurrentSlot = ContainerSlotsArray[i]->SlotStructure;

		/* Update Empty Slot */
		if(CurrentSlot.Amount <= 0){
			CurrentSlot = EmptySlot;
		}
		
		ContainerSlotsArray[i]->UpdateSlot(CurrentSlot);
	}
}


void UContainerLayout::UpdateContainerSlotsUI(uint8 SlotsPerSow, uint8 NumberOfRows)
{
	InventorySize = SlotsPerSow * NumberOfRows;
	
	//ContainerGridPanel->ClearChildren();
	//ContainerSlotsArray.Empty();
	
	FWidgetsLayoutBP* WidgetLayout = Cast<AMyHUD>(PlayerController->MyHUD)->GetWidgetBPClass("SlotLayout_WBP");
	if (WidgetLayout)
	{
		USlotLayout* W_ContainerSlot = nullptr;
		
		for(int i = 0; i < NumberOfRows; i++)
		{
			for(int j = 0; j < SlotsPerSow; j++)
			{
				W_ContainerSlot = CreateWidget<USlotLayout>(GetWorld(), WidgetLayout->Widget);
				ContainerGridPanel->AddChildToUniformGrid(W_ContainerSlot, i, j);
				
				ContainerSlotsArray.Add(W_ContainerSlot);
			}
		}
		
		const FSlotStructure SlotStructure = PlayerController->InventoryManagerComponent->GetEmptySlot(EEquipmentSlot::Undefined);
	
		for(int i = 0; i < ContainerSlotsArray.Num(); i++)
		{
			ContainerSlotsArray[i]->UpdateSlot(SlotStructure);
			ContainerSlotsArray[i]->InventorySlotIndex = i;
			ContainerSlotsArray[i]->NativeFromContainer = true;
		
			ContainerSlotsArray[i]->IsStorageSlot = true;
		}
	}
}