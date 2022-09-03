// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/EquipmentComponent.h"
#include "MyCharacter.h"
#include "Inventory/FSlotStructure.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	EquipmentCharacterReference = nullptr;

	NumberOfRowsInventory = 7;
	SlotsPerRowInventory = 4;

	NumberOfSlotsOnHotbar = 5;
}

// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipmentComponent::UpdateEquippedMeshes(uint8 InventorySlot)
{
	if (IsValid(EquipmentCharacterReference))
	{
		if (InventorySlot < (uint8)EEquipmentSlot::Count)
		{
			FSlotStructure Slot = GetInventoryItem(InventorySlot);
			USkeletalMesh* NewMesh = Slot.ItemStructure.SkeletalMesh;

			switch (InventorySlot)
			{
			case EEquipmentSlot::Weapon:
				EquipmentCharacterReference->MainWeaponMesh = NewMesh;
				EquipmentCharacterReference->OnRep_MainWeaponMesh();
				break;
			case EEquipmentSlot::Chest:
				EquipmentCharacterReference->ChestMesh = NewMesh;
				EquipmentCharacterReference->OnRep_MainChestMesh();
				break;
			case EEquipmentSlot::Feet:
				EquipmentCharacterReference->FeetMesh = NewMesh;
				EquipmentCharacterReference->OnRep_MainFeetMesh();
				break;
			case EEquipmentSlot::Hands:
				EquipmentCharacterReference->HandsMesh = NewMesh;
				EquipmentCharacterReference->OnRep_MainHandsMesh();
				break;
			case EEquipmentSlot::Legs:
				EquipmentCharacterReference->LegsMesh = NewMesh;
				EquipmentCharacterReference->OnRep_MainLegsMesh();
				break;
			case EEquipmentSlot::Head:
				EquipmentCharacterReference->HeadMesh = NewMesh;
				EquipmentCharacterReference->OnRep_MainHeadMesh();
				break;
			default:
				break;
			}
		}
	}
}

void UEquipmentComponent::SetInventoryItem(uint8 InventorySlot, FSlotStructure& Item)
{
	Super::SetInventoryItem(InventorySlot, Item);

	UpdateEquippedMeshes(InventorySlot);
}

void UEquipmentComponent::ClearInventoryItem(uint8 InventorySlot)
{
	Super::ClearInventoryItem(InventorySlot);
	
	UpdateEquippedMeshes(InventorySlot);
}