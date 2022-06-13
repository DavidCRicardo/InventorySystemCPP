// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "MyPlayerController.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Get ItemDB 
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_ItemDB(TEXT("/Game/Blueprints/Item_DB.Item_DB"));
	if (BP_ItemDB.Succeeded())
	{
		ItemDB = BP_ItemDB.Object;
	}else{
		UE_LOG(LogTemp, Warning, TEXT ("ItemDB DataTable not found!!"));
	}
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
	if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetOwner()))
	{
		Inventory.Reserve(Size);

		FSlotStructure SlotStructure = {};
		Inventory.Init(SlotStructure, Size);

	}else
	{
		// Cast Failed on Containers

		// If inventory is empty
		Inventory.Reserve(Size);

		FSlotStructure SlotStructure = GetEmptySlot(EEquipmentSlot::Undefined);
		Inventory.Init(SlotStructure, Size);
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

	/*if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>())
	{
		
	}*/
	FSlotStructure SlotStructure = GetEmptySlot(EEquipmentSlot::Undefined);

	
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

FSlotStructure UInventoryComponent::GetEmptySlot(const EEquipmentSlot FromEquipmentType)
{
	FName Name;
	if (FromEquipmentType == EEquipmentSlot::Weapon)
	{
		Name = "No_Weapon";
	}else if(FromEquipmentType == EEquipmentSlot::Chest)
	{
		Name = "No_Chest";
	}else if(FromEquipmentType == EEquipmentSlot::Feet)
	{
		Name = "No_Feet";
	}else if(FromEquipmentType == EEquipmentSlot::Hands)
	{
		Name = "No_Hands";
	}else
	{
		Name = "Empty";
	}
	
	return GetItemFromItemDB(Name);
}

FSlotStructure UInventoryComponent::GetItemFromItemDB(const FName Name)
{
	FSlotStructure Slot = {};

	const UDataTable* ItemTable = ItemDB;
	const FItemStructure* NewItemData = ItemTable->FindRow<FItemStructure>(FName(Name), "", true);

	Slot.InitSlot(*NewItemData, 0);

	return Slot;
}

void UInventoryComponent::SetInventoryItem(uint8& Index, FSlotStructure& Item)
{
	Inventory[Index] = Item;
}

FSlotStructure UInventoryComponent::GetInventorySlot(uint8 Index)
{
	return Inventory[Index];
}

void UInventoryComponent::ClearInventorySlot(uint8 Index)
{
	// ! Not Tested !
	Inventory[Index].Amount = 0;
}

void UInventoryComponent::PrintInventory()
{
	for (int i = 0; i < Inventory.Num(); i++)
	{
		FText a = Inventory[i].ItemStructure.Name;
		uint8 b = Inventory[i].Amount;

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Item: %s, Amount %i"),*a.ToString(), b));
	}
}