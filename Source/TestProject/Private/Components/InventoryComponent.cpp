// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

DECLARE_LOG_CATEGORY_CLASS(LogInventory, Verbose, Verbose);

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
	UE_LOG(LogInventory, Warning, TEXT ("ItemDB DataTable not found!!"));
	}
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	NumberOfSlots = 28;

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

	const UDataTable* ItemTable = ItemDB;
	for (FSlotStructure& CurrentSlot : Inventory)
	{
		FItemStructure* NewItemData = ItemTable->FindRow<FItemStructure>(FName("Empty"), "", true);
		CurrentSlot.ItemStructure = *NewItemData;
	}
}

bool UInventoryComponent::AddItem(FName ID, uint8 Amount)
{
	const UDataTable* ItemTable = ItemDB;
	FItemStructure* NewItemData = ItemTable->FindRow<FItemStructure>(FName(ID), "", true);

	if (!NewItemData)
	{
		return false;
	}

	FSlotStructure NewSlot = {};
	NewSlot.InitSlot(*NewItemData, Amount);
	
	return AddItemToInventory(NewSlot);
}

bool UInventoryComponent::AddItemToInventory(FSlotStructure& ContentToAdd)
{
	if (ContentToAdd.ItemStructure.IsStackable)
	{
		UE_LOG(LogInventory, Log, TEXT("Item exists on Inventory and it's stackable"));

		const FReturnTupleBoolInt ReturnValue = HasPartialStack(ContentToAdd);
		if (ReturnValue.Success)
		{
			return AddToStack(ContentToAdd, ReturnValue.Index);
		}
	}

	UE_LOG(LogInventory, Display, TEXT("Item doesn't exists on Inventory or it's not stackable"));
	
	if (CreateStack(ContentToAdd))
	{
		return true;
	}
	
	return false;
}

bool UInventoryComponent::CreateStack(const FSlotStructure& ContentToAdd)
{
	bool HasSpace = false;
	uint8 IdentifiedIndex = 0;
	
	for (size_t i = 0; i < NumberOfSlots; i++)
	{
		const FSlotStructure& CurrentSlot = Inventory[i];
		if (CurrentSlot.Amount <= 0)
		{
			HasSpace = true;
			IdentifiedIndex = i;
			break;
		}
	}
	
	if (HasSpace)
	{
		Inventory[IdentifiedIndex] = ContentToAdd;
		return true;
	}
	
	return false;
}

bool UInventoryComponent::AddToStack(FSlotStructure& ContentToAdd, const int8& Index)
{
	const FSlotStructure& CurrentSlot = Inventory[Index];
	const uint8 MaxStackSize = CurrentSlot.ItemStructure.MaxStackSize;
	
	const uint8 FinalQuantity = CurrentSlot.Amount + ContentToAdd.Amount;
	
	if (FinalQuantity > MaxStackSize)
	{
		Inventory[Index].Amount = MaxStackSize;

		const uint8 RestAmountToAdd = ContentToAdd.Amount - (FinalQuantity - MaxStackSize);

		ContentToAdd.Amount = RestAmountToAdd;
		AddItemToInventory(ContentToAdd);
	}else
	{
		Inventory[Index].Amount = FinalQuantity;
	}
	
	return true;
}

FReturnTupleBoolInt UInventoryComponent::HasPartialStack(const FSlotStructure& ContentToAdd)
{
	int8 LocalInteger = -1;
	bool LocalBoolean = false;
	
	for (size_t i = 0; i < NumberOfSlots; i++)
	{
		const bool SameID = Inventory[i].ItemStructure.ID == ContentToAdd.ItemStructure.ID;
		
		const bool InsideStackLimit = Inventory[i].Amount < ContentToAdd.ItemStructure.MaxStackSize;
		
		if (SameID && InsideStackLimit)
		{
			LocalInteger = i;
			LocalBoolean = true;
			break;
		}
	}

	if (LocalBoolean)
	{
		return {true, LocalInteger};
	}
	return {false, 0};
}