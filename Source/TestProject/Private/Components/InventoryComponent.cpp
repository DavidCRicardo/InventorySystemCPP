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
	
	// NumberOfSlots = 28;// + (uint8)EEquipmentSlot::Count;
	NumberOfSlots = 28 + (uint8)EEquipmentSlot::Count;
	
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
	Inventory.Reserve(NumberSlots);

	FSlotStructure SlotStructure = {};
	Inventory.Init(SlotStructure, NumberSlots);

	/*for (int i = 0; i < NumberSlots; i++)
	{
		SlotStructure = GetEmptySlot(GetEquipmentTypeBySlot(i));
		SetInventorySlot(SlotStructure, i);
	}*/
	
	uint8 Index = 0;
	for (FSlotStructure& CurrentSlot : Inventory)
	{
		if (Index == 0)
		{
			SlotStructure = GetEmptySlot(EEquipmentSlot::Weapon);
		}
		else if (Index == 1)
		{
			SlotStructure = GetEmptySlot(EEquipmentSlot::Chest);
		}
		else if (Index == 2)
		{
			SlotStructure = GetEmptySlot(EEquipmentSlot::Feet);
		}
		else if (Index == 3)
		{
			SlotStructure = GetEmptySlot(EEquipmentSlot::Hands);
		}
		else
		{
			SlotStructure = GetEmptySlot(EEquipmentSlot::Undefined);
		}
		
		CurrentSlot = SlotStructure;
		Index++;
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

	// UE_LOG(LogInventory, Display, TEXT("Item doesn't exists on Inventory or it's not stackable"));
	
	if (CreateStack(ContentToAdd))
	{
		return true;
	}
	
	return false;
}

bool UInventoryComponent::CreateStack(FSlotStructure& ContentToAdd)
{
	bool HasSpace = false;
	uint8 IdentifiedIndex = 0;

	// for (size_t i = 0; i < NumberOfSlots; i++)
	for (size_t i = (uint8)EEquipmentSlot::Count; i < NumberOfSlots; i++)
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

		/**/
		const uint8 MaxStackSize = ContentToAdd.ItemStructure.MaxStackSize;
		const uint8 FinalQuantity = ContentToAdd.Amount;
	
		if (FinalQuantity > MaxStackSize)
		{
			Inventory[IdentifiedIndex].Amount = MaxStackSize;
		
			const uint8 RestAmountToAdd = (FinalQuantity - MaxStackSize);

			ContentToAdd.Amount = RestAmountToAdd;
		
			AddItemToInventory(ContentToAdd);
		}else
		{
			Inventory[IdentifiedIndex].Amount = FinalQuantity;
		}
		/**/
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
		
		const uint8 RestAmountToAdd = (FinalQuantity - MaxStackSize);

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

	// for (size_t i = 0; i < NumberOfSlots; i++)
	for (size_t i = (uint8)EEquipmentSlot::Count; i < NumberOfSlots; i++)
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

bool UInventoryComponent::EquipItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot)
{
	// This method should be named ServerEquipFromInventory (Runs on Server and its Reliable)
	// It redirects to EquipItem(FromInventorySlot, ToInventorySlot);
	
	/*const uint8 LocalFromInventorySlot = FromInventorySlot;*/
	/*const uint8 LocalToInventorySlot = ToInventorySlot;*/
	//GetInventoryItem(LocalFromInventorySlot);
	/*const FSlotStructure LocalInventoryItem = GetInventorySlot(FromInventorySlot);*/
	
	if (GetItemTypeBySlot(FromInventorySlot) == EItemType::Equipment)
	{
		EEquipmentSlot LocalEquipmentSlotType = GetEquipmentTypeBySlot(FromInventorySlot); //LocalInventoryItem.ItemStructure.EquipmentSlot;
		 
		EEquipmentSlot EquipmentType = GetEquipmentTypeBySlot(ToInventorySlot);
		if (EquipmentType == LocalEquipmentSlotType)
		{
			const FSlotStructure LocalSlot = GetInventorySlot(FromInventorySlot);
			const FSlotStructure SwapSlot = GetInventorySlot(ToInventorySlot);

			SetInventorySlot(LocalSlot, ToInventorySlot);
			SetInventorySlot(SwapSlot, FromInventorySlot);
			/*
			//GetInventoryItem(LocalToInventorySlot);
			const FSlotStructure LocalSwapInventoryItem = GetInventorySlot(LocalToInventorySlot);
			// Swap Items
			if(LocalSwapInventoryItem.Amount > 0)
			{
				//if(CanContainerStoreItems())
				//{
					// Swap Items
				//}else
				//{
					// Warning: Container Cannot Store Items
				//	return;
				//}
			}else
			{
				SetInventorySlot(LocalInventoryItem,LocalToInventorySlot);
				RemoveItem(LocalFromInventorySlot);
				// Move Item (Add Item, Remove Item)
			}
			// UpdateEquippedStats()
			*/
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Let's Equip that Item!!")));

			return true;
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ITEM CAN NOT BE EQUIPPED HERE")));
		return false;
	}
	
	// Warning: Item is not Equippable
	return false;
}

/* After drag the Item back to Inventory, it needs to refresh correctly the equipment empty slot */
/* Depending if can swap items (both same equipment type) or if it will need to change to the correctly empty slot */
bool UInventoryComponent::UnEquipItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot)
{
	if (FromInventorySlot != ToInventorySlot)
	{
		if (GetItemTypeBySlot(ToInventorySlot) == EItemType::Undefined)
		{
			const FSlotStructure LocalSlot = GetInventorySlot(FromInventorySlot);
			const FSlotStructure SwapSlot = GetInventorySlot(ToInventorySlot);

			SetInventorySlot(LocalSlot, ToInventorySlot);
			SetInventorySlot(SwapSlot, FromInventorySlot);

			return true;
		}
	}
	
	return false;
}

bool UInventoryComponent::MoveInventoryItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot)
{
	// Trying to Move to Different Spot
	if (FromInventorySlot != ToInventorySlot)
	{
		const FSlotStructure LocalSlot = GetInventorySlot(FromInventorySlot);
		const FSlotStructure SwapSlot = GetInventorySlot(ToInventorySlot);

		SetInventorySlot(LocalSlot, ToInventorySlot);
		SetInventorySlot(SwapSlot, FromInventorySlot);

		return true;
	}
	return false;
}

void UInventoryComponent::SetInventorySlot(const FSlotStructure& ContentToAdd, const uint8& InventorySlot)
{
	Inventory[InventorySlot] = ContentToAdd;
}

FSlotStructure UInventoryComponent::GetInventorySlot(const uint8& InventorySlot)
{
	if (Inventory[InventorySlot].Amount > 0)
	{
		return Inventory[InventorySlot];
	}
	
	return GetEmptySlot(GetEquipmentTypeBySlot(InventorySlot));
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

void UInventoryComponent::UseInventoryItem(const uint8& InventorySlot)
{
	FSlotStructure LocalInventorySlot = GetInventorySlot(InventorySlot);

	switch (LocalInventorySlot.ItemStructure.ItemType)
	{
	case EItemType::Consumable:
		UseConsumableItem(InventorySlot, LocalInventorySlot);
		break;
	default:
		break;
	}
}

void UInventoryComponent::UseConsumableItem(const uint8& InventorySlot, FSlotStructure& InventoryItem)
{
	// Do something depending on the item properties if needed
	// ...

	// Remove from Item Amount
	uint8 AmountToRemove = 1;
	bool WasFullAmountRemoved = false;
	uint8 AmountRemoved = 0;
	
	RemoveFromItemAmount(InventoryItem, AmountToRemove, WasFullAmountRemoved, AmountRemoved);

	if (WasFullAmountRemoved)
	{
		InventoryItem = GetEmptySlot(EEquipmentSlot::Undefined);

		RemoveItem(InventorySlot);
	}else
	{
		Inventory[InventorySlot] = InventoryItem;
	}
}

void UInventoryComponent::RemoveFromItemAmount(FSlotStructure& InventoryItem, const uint8& AmountToRemove,
	bool& WasFullAmountRemoved, uint8& AmountRemoved)
{
	if (AmountToRemove >= InventoryItem.Amount)
	{
		AmountRemoved = InventoryItem.Amount;
		
		WasFullAmountRemoved = true;
	}else
	{
		const uint8 UpdatedAmount = InventoryItem.Amount - AmountToRemove;
		InventoryItem.Amount = UpdatedAmount;

		WasFullAmountRemoved = false;
	}
}

void UInventoryComponent::RemoveItem(const uint8& InventorySlot)
{
	// Clear Inventory Item
	Inventory[InventorySlot] = GetEmptySlot(GetEquipmentTypeBySlot(InventorySlot));


	// Clear Inventory Slot Item UI
	//RefreshInventoryUI();
	//Inventory UI  - Inventory Slots .get(InventorySlot) = GetEmptySlot();
}

void UInventoryComponent::ClearInventorySlot(const uint8& InventorySlot)
{
	Inventory[InventorySlot] = GetEmptySlot(GetEquipmentTypeBySlot(InventorySlot));
}

// Returns the equipment type by equip slot number.
EEquipmentSlot UInventoryComponent::GetEquipmentTypeBySlot(const uint8& EquipmentSlot)
{
	return Inventory[EquipmentSlot].ItemStructure.EquipmentSlot;
	switch (EquipmentSlot)
	{
	case 0:
		return EEquipmentSlot::Weapon;
	case 1:
		return EEquipmentSlot::Chest;
	case 2:
		return EEquipmentSlot::Feet;
	case 3:
		return EEquipmentSlot::Hands;
	default:
		return EEquipmentSlot::Undefined;
	}
}

EItemType UInventoryComponent::GetItemTypeBySlot(const uint8& ItemSlot)
{
	return Inventory[ItemSlot].ItemStructure.ItemType;
	switch (ItemSlot)
	{
	case 1:
		return EItemType::Miscellaneous;
	case 2:
		return EItemType::Equipment;
	case 3:
		return EItemType::Consumable;
	default:
		return EItemType::Undefined;
	}
}