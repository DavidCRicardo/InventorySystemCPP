// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "MyPlayerController.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::Server_InitInventory_Implementation(const uint8& InventorySize)
{
	InitInventory(InventorySize);
}

void UInventoryComponent::InitInventory(const uint8& Size)
{
	if( AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetOwner()))
	{
		/*Inventory.Empty();
	const FSlotStructure LocalSlot = {};
	
	for (uint8 Index = 0; Index < (Size - 1); Index++)
	{
		Inventory.Add(LocalSlot);
	}*/

	Inventory.Reserve(Size);

	FSlotStructure SlotStructure = {};
	Inventory.Init(SlotStructure, Size);

	// Add Customized Icons to Slots
	uint8 Index = 0;
	for (FSlotStructure& CurrentSlot : Inventory)
	{
		if (Index == 0)
		{
			SlotStructure = PlayerController->InventoryManagerComponent->GetEmptySlot(EEquipmentSlot::Weapon);
		}
		else if (Index == 1)
		{
			SlotStructure = PlayerController->InventoryManagerComponent->GetEmptySlot(EEquipmentSlot::Chest);
		}
		else if (Index == 2)
		{
			SlotStructure = PlayerController->InventoryManagerComponent->GetEmptySlot(EEquipmentSlot::Feet);
		}
		else if (Index == 3)
		{
			SlotStructure = PlayerController->InventoryManagerComponent->GetEmptySlot(EEquipmentSlot::Hands);
		}
		else
		{
			// Default Icon
			SlotStructure = PlayerController->InventoryManagerComponent->GetEmptySlot(EEquipmentSlot::Undefined);
		}
		
		CurrentSlot = SlotStructure;
		Index++;
	}
		
	}
}

void UInventoryComponent::GetInventoryItems(TArray<FSlotStructure>& InventoryItems)
{
	InventoryItems = Inventory;
}

bool UInventoryComponent::LoadInventoryItems(uint8 Size, TArray<FSlotStructure> Array)
{
	Inventory.Reset();
	FSlotStructure TempSlot = {};
	
	for (uint8 i = 0; i < Size - 1; i++)
	{
		Inventory.Add(TempSlot);
	}

	uint8 ArrayIndex = 0;
	for (FSlotStructure Slot : Array)
	{
		SetInventoryItem(ArrayIndex, Slot);
		ArrayIndex++;
	}

	return true;
}

void UInventoryComponent::SetInventoryItem(uint8& Index, FSlotStructure& Item)
{
	Inventory[Index] = Item;
}

FSlotStructure UInventoryComponent::GetInventorySlot(uint8 Index)
{
	return Inventory[Index];
}
