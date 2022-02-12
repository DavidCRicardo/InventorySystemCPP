// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/EquipmentComponent.h"


// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	NumberOfEquipmentSlots = 4;

	InitInventory(NumberOfEquipmentSlots);
}


// Called every frame
void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipmentComponent::InitInventory(int32 NumberSlots)
{
	Super::InitInventory(NumberSlots);
	
}

bool UEquipmentComponent::MoveInventoryItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot)
{
	// Trying to Equip an Item
	if (FromInventorySlot != ToInventorySlot)
	{
		FSlotStructure LocalSlot = GetItemFromInventory(FromInventorySlot);
		FSlotStructure SwapSlot = GetItemFromInventory(ToInventorySlot);

		AddItemToIndex(LocalSlot, ToInventorySlot);
		AddItemToIndex(SwapSlot, FromInventorySlot);

		return true;
	}
	return false; //Super::MoveInventoryItem(FromInventorySlot, ToInventorySlot);
}

bool UEquipmentComponent::EquipItem(FSlotStructure& SlotStructure)
{
	return false;
}
