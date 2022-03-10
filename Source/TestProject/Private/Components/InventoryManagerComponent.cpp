// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryManagerComponent.h"

#include "WorldActor.h"

DECLARE_LOG_CATEGORY_CLASS(LogInventory, Verbose, Verbose);

// Sets default values for this component's properties
UInventoryManagerComponent::UInventoryManagerComponent()
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
void UInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// NumberOfSlots = 28;// + (uint8)EEquipmentSlot::Count;
	NumberOfSlots = 28 + (uint8)EEquipmentSlot::Count;
	
	InitInventory(NumberOfSlots);
}


// Called every frame
void UInventoryManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

/* Initializes the Inventory Array to a Specified Size */
void UInventoryManagerComponent::InitInventory(const int32 NumberSlots)
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

bool UInventoryManagerComponent::AddItem(FName ID, uint8 Amount)
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

bool UInventoryManagerComponent::AddItemToInventory(FSlotStructure& ContentToAdd)
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

bool UInventoryManagerComponent::CreateStack(FSlotStructure& ContentToAdd)
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

bool UInventoryManagerComponent::AddToStack(FSlotStructure& ContentToAdd, const int8& Index)
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

FReturnTupleBoolInt UInventoryManagerComponent::HasPartialStack(const FSlotStructure& ContentToAdd)
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



bool UInventoryManagerComponent::EquipItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot)
{
	// This method should be named ServerEquipFromInventory (Runs on Server and its Reliable)
	// It redirects to EquipItem(FromInventorySlot, ToInventorySlot);
	
	/*const uint8 LocalFromInventorySlot = FromInventorySlot;*/
	/*const uint8 LocalToInventorySlot = ToInventorySlot;*/
	//GetInventoryItem(LocalFromInventorySlot);
	/*const FSlotStructure LocalInventoryItem = GetInventorySlot(FromInventorySlot);*/
	
	if (GetItemTypeBySlot(FromInventorySlot) == EItemType::Equipment)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Equip this item...")));

		EEquipmentSlot LocalEquipmentSlotType = GetEquipmentTypeBySlot(FromInventorySlot);
		 
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

			UpdateEquippedMeshes(ToInventorySlot);

			return true;
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ITEM CAN NOT BE EQUIPPED HERE")));
		return false;
	}
	
	// Warning: Item is not Equippable
	return false;
}

bool UInventoryManagerComponent::UnEquipItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot)
{
	// Trying to Move to Different Spot
	if (FromInventorySlot != ToInventorySlot)
	{
	    /* ToInventorySlot is an Empty Slot*/
		if (GetItemTypeBySlot(ToInventorySlot) == EItemType::Undefined)
		{
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Unequip and left empty equip slot")));

			const FSlotStructure LocalSlot = GetInventorySlot(FromInventorySlot);
			const FSlotStructure SwapSlot = GetInventorySlot(ToInventorySlot);
		
			SetInventorySlot(LocalSlot, ToInventorySlot);
			SetInventorySlot(SwapSlot, FromInventorySlot);

			UpdateEquippedMeshes(FromInventorySlot);
			UpdateEquippedMeshes(ToInventorySlot);
			
			return true;
		}
		
		/* Can only swap with a non Empty Slot if this one is an Equipment from the same EquipType */
	    if( GetItemTypeBySlot(ToInventorySlot) == GetItemTypeBySlot(FromInventorySlot) )
	    {
		    if (GetEquipmentTypeBySlot(ToInventorySlot) == GetEquipmentTypeBySlot(FromInventorySlot))
		    {
		    	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Unequip and Swap w/ another equip")));

		    	EquipItem(ToInventorySlot, FromInventorySlot);
			    /*const FSlotStructure LocalSlot = GetInventorySlot(FromInventorySlot);
			    const FSlotStructure SwapSlot = GetInventorySlot(ToInventorySlot);
            		
			    SetInventorySlot(LocalSlot, ToInventorySlot);
			    SetInventorySlot(SwapSlot, FromInventorySlot);

		    	UpdateEquippedMeshes(FromInventorySlot);
		    	UpdateEquippedMeshes(ToInventorySlot);*/

			    return true;
		    }
	    }
		
		
	    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("YOU CANNOT DO THAT HERE")));
	}
	
	return false;
}

void UInventoryManagerComponent::DropItem(const uint8& InventorySlot)
{
	FSlotStructure LocalSlot = GetInventorySlot(InventorySlot);
	if (LocalSlot.ItemStructure.IsDroppable)
	{
		FTransform OutTransform {};
		// Randomize Drop Location (&OutTransform)
		if (APlayerController* PC =  Cast<APlayerController>(GetOwner()) )
		{
			// Drop at character feet
			FVector LocalLocation {0.0f, 0.0f, -96.0f};
			
			FVector PawnLocation = PC->GetPawn()->GetActorLocation();

			//Drop Distance Range From Character
			const uint8 DropDistanceRange = FMath::RandRange(64, 96);
			FVector LocationOnRange {(float)DropDistanceRange,0.0f,0.0f};

			// Drop Items 360 Degrees Around Player
			const uint8 DropItemsRotation = FMath::RandRange(0, 360); // -100 , 100
			FRotator Rotation {0.0f, 0.0f, (float)DropItemsRotation};

			FVector VectorRotated = Rotation.RotateVector(LocationOnRange);

			FVector FinalLocation = PawnLocation + VectorRotated + LocalLocation;

			// Give The Dropped Object A Random Rotation
			const int8 RandomRotation = FMath::RandRange(-10, 10);

			FRotator FinalRotator {0.0f, 0.0f, (float)RandomRotation * 10};

			FVector FinalScale {1.0f,1.0f,1.0f};

			OutTransform = {FinalRotator, FinalLocation, FinalScale};
		}
		
		// Spawn World Actor
		UClass* ActorClass = LocalSlot.ItemStructure.Class;
		AWorldActor* WActor = GetWorld()->SpawnActor<AWorldActor>(ActorClass, OutTransform);
		if (WActor )
		{
			//PActor->ID = LocalSlot.ItemStructure.ID;
			WActor->Amount = LocalSlot.Amount;
		}

		RemoveItem(InventorySlot);

		// Are we dropping an equipped item?
		if (InventorySlot < (uint8)EEquipmentSlot::Count)
		{
			UpdateEquippedMeshes(InventorySlot);
		}
	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("You cannot drop this..."))
	}
}

bool UInventoryManagerComponent::MoveInventoryItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot)
{
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

void UInventoryManagerComponent::SetInventorySlot(const FSlotStructure& ContentToAdd, const uint8& InventorySlot)
{
	Inventory[InventorySlot] = ContentToAdd;
}

FSlotStructure UInventoryManagerComponent::GetInventorySlot(const uint8& InventorySlot)
{
	if (Inventory[InventorySlot].Amount > 0)
	{
		return Inventory[InventorySlot];
	}
	
	return GetEmptySlot(GetEquipmentTypeBySlot(InventorySlot));
}

FSlotStructure UInventoryManagerComponent::GetEmptySlot(const EEquipmentSlot FromEquipmentType)
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

FSlotStructure UInventoryManagerComponent::GetItemFromItemDB(const FName Name)
{
	FSlotStructure Slot = {};

	const UDataTable* ItemTable = ItemDB;
	const FItemStructure* NewItemData = ItemTable->FindRow<FItemStructure>(FName(Name), "", true);

	Slot.InitSlot(*NewItemData, 0);

	return Slot;
}

void UInventoryManagerComponent::UseInventoryItem(const uint8& InventorySlot)
{
	FSlotStructure LocalInventorySlot = GetInventorySlot(InventorySlot);

	switch (LocalInventorySlot.ItemStructure.ItemType)
	{
	case EItemType::Consumable:
		UseConsumableItem(InventorySlot, LocalInventorySlot);
		break;
	case EItemType::Equipment:
		UseEquipmentItem(InventorySlot, LocalInventorySlot);
		break;
	default:
		break;
	}
}

void UInventoryManagerComponent::UseConsumableItem(const uint8& InventorySlot, FSlotStructure& InventoryItem)
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


void UInventoryManagerComponent::UseEquipmentItem(const uint8& InventorySlot, const FSlotStructure& SlotStructure)
{
	// Are we Unequipping?
	const uint8 NumberOfEntries = (uint8)EEquipmentSlot::Count;
	if (InventorySlot < NumberOfEntries)
	{
		// Yes, we are Unequipping
		uint8 Index = 0;
		if (GetEmptyInventorySpace(Index))
		{
				Server_UnEquipFromInventory_Implementation(InventorySlot, Index);
			return;	
		}
		UE_LOG(LogInventory, Warning, TEXT("NO FREE SPACE"))
	}
	else
	{
		EEquipmentSlot LocalEquipmentSlot = GetItemEquipmentSlot(SlotStructure);
		uint8 Index = 0;
		// Finds Empty Slot Of Type To Equip 
		if( GetEmptyEquipmentSlotByType(LocalEquipmentSlot, Index) )
		{
			Server_EquipFromInventory_Implementation(InventorySlot, Index);
		}else
		{
			const uint8 ToInventorySlot = GetEquipmentSlotByType(LocalEquipmentSlot);
			Server_EquipFromInventory_Implementation(InventorySlot, ToInventorySlot);
		}
	}

	//No, Find Empty Slot Of Type To Equip To
	
}
bool UInventoryManagerComponent::GetEmptyInventorySpace(uint8& OutIndex)
{
	for(uint8 Index = (uint8)EEquipmentSlot::Count; Index < NumberOfSlots; Index++)
	{
		FSlotStructure Slot = Inventory[Index];
		if (!ItemIsValid(Slot))
		{
			OutIndex = Index;
			return true;
		}
	}
	return false;
}

EEquipmentSlot UInventoryManagerComponent::GetItemEquipmentSlot(const FSlotStructure Item)
{
	return Item.ItemStructure.EquipmentSlot;
}

bool UInventoryManagerComponent::ItemIsValid(const FSlotStructure Item)
{
	if (Item.Amount > 0)
	{
		return true;
	}
	return false;
}

bool UInventoryManagerComponent::GetEmptyEquipmentSlotByType(EEquipmentSlot EquipmentSlot, uint8& OutIndex)
{
	for (uint8 Index = 0; Index < (uint8)EEquipmentSlot::Count; Index++)
	{
		EEquipmentSlot LocalEquipSlot = GetEquipmentTypeBySlot(Index);
		FSlotStructure LocalSlot = GetInventorySlot(Index);

		// Is Same Type And Empty?
		if (!ItemIsValid(LocalSlot) && EquipmentSlot == LocalEquipSlot)
		{
			OutIndex = Index;
			return true;;
		}
	}
	
	return false;
}

void UInventoryManagerComponent::RemoveFromItemAmount(FSlotStructure& InventoryItem, const uint8& AmountToRemove,
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

void UInventoryManagerComponent::RemoveItem(const uint8& InventorySlot)
{
	// Clear Inventory Item
	Inventory[InventorySlot] = GetEmptySlot(GetEquipmentTypeBySlot(InventorySlot));


	// Clear Inventory Slot Item UI
	//RefreshInventoryUI();
	//Inventory UI  - Inventory Slots .get(InventorySlot) = GetEmptySlot();
}

void UInventoryManagerComponent::ClearInventorySlot(const uint8& InventorySlot)
{
	Inventory[InventorySlot] = GetEmptySlot(GetEquipmentTypeBySlot(InventorySlot));
}

void UInventoryManagerComponent::UpdateEquippedMeshes(const uint8& InventorySlot)
{
	if (IsValid(CharacterReference))
	{
		if (InventorySlot >= (uint8)EEquipmentSlot::Count)
		{
			return;
		}

		FSlotStructure Slot = GetInventorySlot(InventorySlot);

		USkeletalMesh* NewMesh = Slot.ItemStructure.SkeletalMesh;

		switch (InventorySlot)
		{
		case EEquipmentSlot::Weapon:
			CharacterReference->Server_UpdateWeaponMesh(NewMesh);
			break;
		case EEquipmentSlot::Chest:
			CharacterReference->Server_UpdateChestMesh(NewMesh);
			break;
		case EEquipmentSlot::Feet:
			CharacterReference->Server_UpdateFeetMesh(NewMesh);
			break;
		case EEquipmentSlot::Hands:
			CharacterReference->Server_UpdateHandsMesh(NewMesh);
			break;
		default:
			break;
		}
	}
}

uint8 UInventoryManagerComponent::GetEquipmentSlotByType(EEquipmentSlot EquipmentSlot)
{
	for(uint8 Index = 0; Index < (uint8)EEquipmentSlot::Count; Index++)
	{
		EEquipmentSlot LocalSlotType = GetEquipmentTypeBySlot(Index);
		if (EquipmentSlot == LocalSlotType)
		{
			return Index;
		}
	}
	return 0;
}

EEquipmentSlot UInventoryManagerComponent::GetEquipmentTypeBySlot(const uint8& EquipmentSlot)
{
	return Inventory[EquipmentSlot].ItemStructure.EquipmentSlot;
}

EItemType UInventoryManagerComponent::GetItemTypeBySlot(const uint8& ItemSlot)
{
	return Inventory[ItemSlot].ItemStructure.ItemType;
}

bool UInventoryManagerComponent::Server_DropItemFromInventory_Validate(uint8 InventorySlot)
{
	return true;
}

void UInventoryManagerComponent::Server_DropItemFromInventory_Implementation(uint8 InventorySlot)
{
	DropItem(InventorySlot);
}

void UInventoryManagerComponent::Server_EquipFromInventory_Implementation(const uint8& FromInventorySlot,
                                                                          const uint8& ToInventorySlot)
{
	EquipItem(FromInventorySlot, ToInventorySlot);
}

void UInventoryManagerComponent::Server_UnEquipFromInventory_Implementation(const uint8& FromInventorySlot,
	const uint8& ToInventorySlot)
{
	UnEquipItem(FromInventorySlot, ToInventorySlot);
}