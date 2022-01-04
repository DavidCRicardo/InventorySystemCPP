// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

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
	UE_LOG(LogTemp, Error, TEXT ("ItemDB not found!!"));
	}
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	NumberOfSlots = 32;

	InitInventory(NumberOfSlots);
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

/* Initializes the Inventory Array to a Specified Size */
void UInventoryComponent::InitInventory(const int32 NumberSlots)
{
	/* Clear Inventory */
	//Inventory.Empty(NumberSlots);
	//Inventory.Reset(NumberSlots);
	
	/* Resize Inventory */
	Inventory.Reserve(NumberSlots);
	
	FSlotStructure SlotStructure = {};
	Inventory.Init(SlotStructure, NumberSlots);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("InventoryComponent Initialized!!"));

	UE_LOG(LogTemp, Warning, TEXT ("InventoryComponent Initialized!!"));
}

bool UInventoryComponent::AddItem(FName ID, uint8 Amount)
{
	const UDataTable* ItemTable = ItemDB;// = GetItemDB();

	FItemStructure* NewItemData = ItemTable->FindRow<FItemStructure>(FName(ID), "", true);
	
	//const FSlotStructure NewItem = {*NewItemData, Amount};
	const FSlotStructure* NewItem = nullptr;
	
	//FSlotStructure* EmptyItem = NewItem;

	//NewItem->InitItem(FName(ID), Amount, *NewItemData);
	/*
	TArray<FSlotStructure>::ElementType NewItem;
	NewItem.ItemStructure = *NewItemData;
	NewItem.Amount = Amount;
	*/
	if (NewItemData)
	{
		/*
		// Check if there is already an item to Stack
		if (Inventory.Contains(NewItem))
		{
			const uint8 Index = Inventory.Find(NewItem);
			Inventory.EmplaceAt(Index, NewItem);
		}else
		{
			// Check if there is an empty slot to Add
			if (Inventory.Contains(nullptr))
			{
				const uint8 Index = Inventory.Find(nullptr);
				Inventory.EmplaceAt(Index, NewItem);
			}
		}
		*/

		Inventory.Add(*NewItem);
		//Inventory.AddUnique(NewItem);
		return true;
	}

	return false;
	
	/*if (ItemToAdd)
	{
		// If you dont want a Slot- or WeightLimit you have to remove it in this line
		if (Inventory.Num() < InventorySlotLimit && GetInventoryWeight() + ItemToAdd->Weight <= InventoryWeightLimit)
		{
			Inventory.Add(*ItemToAdd);
			ReloadInventory();
			return true;
		}
	}
	return false;*/
}

bool UInventoryComponent::HasEmptySlots()
{
	return true;
}

bool UInventoryComponent::CreateStack()
{
	return true;
}

bool UInventoryComponent::AddToStack()
{
	return true;
}

int8 UInventoryComponent::HasPartialStack(FSlotStructure SlotStructure)
{
	int8 StackIndex = -1;
	if (true)
	{
		return StackIndex;
	}else
	{
		return StackIndex;
	}
}

	
/*for (AItem*& CurrentItem : Inventory)
{
	Inventory.Add(nullptr);
}*/
	
/*for (size_t i = 0; i < NumberSlots; i++)
{
	AItem* EmptyItem = NewObject<AItem>();
	Inventory[i] = EmptyItem;
}*/