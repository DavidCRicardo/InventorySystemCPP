// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"


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
	Inventory.Empty();
	const FSlotStructure LocalSlot = {};
	
	for (uint8 Index = 0; Index < (Size - 1); Index++)
	{
		Inventory.Add(LocalSlot);
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
		//Inventory[ArrayIndex] = Slot;
	}

	return true;
}

void UInventoryComponent::SetInventoryItem(uint8& Index, FSlotStructure& Item)
{
	Inventory[Index] = Item;
}
