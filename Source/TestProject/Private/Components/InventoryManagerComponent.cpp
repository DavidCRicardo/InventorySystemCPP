// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryManagerComponent.h"

#include "ContainerActor.h"
#include "MyHUD.h"
#include "MyPlayerController.h"
#include "Components/UniformGridPanel.h"
#include "GameFramework/PlayerState.h"
#include "Inventory/FContainerInfo.h"
#include "Net/UnrealNetwork.h"
#include "UI/ContainerLayout.h"
#include "UI/InventoryLayout.h"

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
	UE_LOG(LogTemp, Warning, TEXT ("ItemDB DataTable not found!!"));
	}
	
	NumberOfSlots = 28 + (uint8)EEquipmentSlot::Count;
}

// Called when the game starts
void UInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UInventoryManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryManagerComponent, NumberOfSlots);
}

void UInventoryManagerComponent::FindAndAddAmountToStacks(UInventoryComponent* Inventory, FName ItemID, uint8 Amount, uint8& AmountRemaining)
{
	uint8 LocalAmount = Amount;
	FName LocalItemID = ItemID;
	uint8 LocalInventorySlot = 4;

	for (uint8 i = 4; i < 32; i++)
	{
		if (LocalItemID == Inventory->Inventory[LocalInventorySlot].ItemStructure.ID)
		{
			AddItemToStack(Inventory, LocalInventorySlot, LocalAmount, AmountRemaining);
			LocalAmount = AmountRemaining;
			
			if (LocalAmount == 0)
			{
				break;
			}
		}	
		LocalInventorySlot++;
	}
	
	AmountRemaining = LocalAmount;
}

void UInventoryManagerComponent::TryToAddItemToInventory(UInventoryComponent* Inventory, FSlotStructure InventoryItem,
                                                         bool& bOutSuccess)
{
	FName LocalItemID = InventoryItem.ItemStructure.ID;
	uint8 LocalItemAmount = InventoryItem.Amount;
	FSlotStructure LocalInventoryItem = InventoryItem;
	UInventoryComponent* LocalInventory = Inventory;

	uint8 AmountRemaining = LocalItemAmount;

	if (LocalInventoryItem.ItemStructure.IsStackable)
	{
		FindAndAddAmountToStacks(LocalInventory, LocalItemID, LocalItemAmount, AmountRemaining);
		LocalItemAmount = AmountRemaining;
		if (LocalItemAmount == 0)
		{
			bOutSuccess = true;
			return;
		}
		else
		{
			LocalInventoryItem.Amount = LocalItemAmount;
		}
	}

	FReturnTupleBoolInt LocalTuple = LocalInventory->GetEmptyInventorySpace();

	if (LocalTuple.Success)
	{
		AddItem2(LocalInventory, LocalTuple.Index, LocalInventoryItem);
		bOutSuccess = true;
		return;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Inventory Full")));
		UE_LOG(LogTemp, Verbose, TEXT("Inventory Full"))

		bOutSuccess = false;
		return;
	}
}

void UInventoryManagerComponent::Server_MoveInventoryItem_Implementation(uint8 FromInventorySlot, uint8 ToInventorySlot)
{
	MoveItem(PlayerInventory, FromInventorySlot, PlayerInventory, ToInventorySlot);
}

void UInventoryManagerComponent::InitializeInventoryManager(UInventoryComponent* EquipmentComponent)
{
	PlayerInventory = EquipmentComponent;
}

UDataTable* UInventoryManagerComponent::GetItemDB()
{
	return ItemDB;
}

/* Initializes the Inventory Array to a Specified Size */
bool UInventoryManagerComponent::InitInventory(const uint8 NumberSlots)
{
	PlayerInventory->Inventory.Reserve(NumberSlots);

	FSlotStructure SlotStructure = {};
	PlayerInventory->Inventory.Init(SlotStructure, NumberSlots);

	// Add Customized Icons to Slots
	uint8 Index = 0;
	for (FSlotStructure& CurrentSlot : PlayerInventory->Inventory)
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
			// Default Icon
			SlotStructure = GetEmptySlot(EEquipmentSlot::Undefined);
		}
		
		CurrentSlot = SlotStructure;
		Index++;
	}
	
	return true;
}

void UInventoryManagerComponent::Server_InitInventory_Implementation()
{
	// careful here
	PlayerInventory->Server_InitInventory(NumberOfSlots);
	//PlayerInventory->Server_InitInventory_Implementation(NumberOfSlots);
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
		UE_LOG(LogTemp, Log, TEXT("Item exists on Inventory and it's stackable"));

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

	for (size_t i = (uint8)EEquipmentSlot::Count; i < NumberOfSlots; i++)
	{
		const FSlotStructure& CurrentSlot = PlayerInventory->Inventory[i];
		if (CurrentSlot.Amount <= 0)
		{
			HasSpace = true;
			IdentifiedIndex = i;
			break;
		}
	}
	
	if (HasSpace)
	{
		PlayerInventory->Inventory[IdentifiedIndex] = ContentToAdd;

		/**/
		const uint8 MaxStackSize = ContentToAdd.ItemStructure.MaxStackSize;
		const uint8 FinalQuantity = ContentToAdd.Amount;
	
		if (FinalQuantity > MaxStackSize)
		{
			PlayerInventory->Inventory[IdentifiedIndex].Amount = MaxStackSize;
		
			const uint8 RestAmountToAdd = (FinalQuantity - MaxStackSize);

			ContentToAdd.Amount = RestAmountToAdd;
		
			AddItemToInventory(ContentToAdd);
		}else
		{
			PlayerInventory->Inventory[IdentifiedIndex].Amount = FinalQuantity;
		}
		/**/
		return true;
	}
	
	return false;
}

bool UInventoryManagerComponent::AddToStack(FSlotStructure& ContentToAdd, const int8& Index)
{
	const FSlotStructure& CurrentSlot = PlayerInventory->Inventory[Index];
	const uint8 MaxStackSize = CurrentSlot.ItemStructure.MaxStackSize;
	
	const uint8 FinalQuantity = CurrentSlot.Amount + ContentToAdd.Amount;
	
	if (FinalQuantity > MaxStackSize)
	{
		PlayerInventory->Inventory[Index].Amount = MaxStackSize;
		
		const uint8 RestAmountToAdd = (FinalQuantity - MaxStackSize);

		ContentToAdd.Amount = RestAmountToAdd;
		
		AddItemToInventory(ContentToAdd);
	}else
	{
		PlayerInventory->Inventory[Index].Amount = FinalQuantity;
	}
	
	return true;
}

FReturnTupleBoolInt UInventoryManagerComponent::HasPartialStack(const FSlotStructure& ContentToAdd)
{
	int8 LocalInteger = -1;
	bool LocalBoolean = false;

	for (size_t i = (uint8)EEquipmentSlot::Count; i < NumberOfSlots; i++)
	{
		const bool SameID = PlayerInventory->Inventory[i].ItemStructure.ID == ContentToAdd.ItemStructure.ID;
		
		const bool InsideStackLimit = PlayerInventory->Inventory[i].Amount < ContentToAdd.ItemStructure.MaxStackSize;
		
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

// This method should be named ServerEquipFromInventory (Runs on Server and its Reliable)
// It redirects to EquipItem(FromInventorySlot, ToInventorySlot);
bool UInventoryManagerComponent::EquipItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot)
{
	if (GetItemTypeBySlot(FromInventorySlot) == EItemType::Equipment)
	{
		EEquipmentSlot LocalEquipmentSlotType = GetEquipmentTypeBySlot(FromInventorySlot);
		 
		EEquipmentSlot EquipmentType = GetEquipmentTypeBySlot(ToInventorySlot);
		if (EquipmentType == LocalEquipmentSlotType)
		{
			const FSlotStructure LocalSlot = GetInventorySlot(FromInventorySlot);
			const FSlotStructure SwapSlot = GetInventorySlot(ToInventorySlot);
			
			SetInventorySlotItem(LocalSlot, ToInventorySlot);
			SetInventorySlotItem(SwapSlot, FromInventorySlot);

			UpdateEquippedMeshes(ToInventorySlot);
			UpdateEquippedStats();
			
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
			const FSlotStructure LocalSlot = GetInventorySlot(FromInventorySlot);
			const FSlotStructure SwapSlot = GetInventorySlot(ToInventorySlot);
		
			SetInventorySlotItem(LocalSlot, ToInventorySlot);
			SetInventorySlotItem(SwapSlot, FromInventorySlot);

			UpdateEquippedMeshes(FromInventorySlot);
			UpdateEquippedMeshes(ToInventorySlot);
			UpdateEquippedStats();
			
			return true;
		}
		
		/* Can only swap with a non Empty Slot if this one is an Equipment from the same EquipType */
	    if( GetItemTypeBySlot(ToInventorySlot) == GetItemTypeBySlot(FromInventorySlot) )
	    {
		    if (GetEquipmentTypeBySlot(ToInventorySlot) == GetEquipmentTypeBySlot(FromInventorySlot))
		    {
		    	EquipItem(ToInventorySlot, FromInventorySlot);
		    	UpdateEquippedStats();
		    	
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
		AMyPlayerController* PC =  Cast<AMyPlayerController>(GetOwner());
		if (!IsValid(PC))
		{
			return;
		}
		
		PC->Server_OnActorDropped(LocalSlot);
		
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

void UInventoryManagerComponent::MoveItem(UInventoryComponent* FromInventory, uint8 FromInventorySlot,
                                          UInventoryComponent* ToInventory, uint8 ToInventorySlot)
{
	// Trying to Move to the Same Spot
	if (FromInventory == ToInventory && FromInventorySlot == ToInventorySlot)
	{
		return;
	}

	FSlotStructure LocalInventoryItem = FromInventory->GetInventorySlot(FromInventorySlot);
	FSlotStructure LocalSwapInventoryItem = ToInventory->GetInventorySlot(ToInventorySlot);

	// Can Container Store Items?
	// Are We Looting Currency?
	// Are We Swapping Items?
	
	// Swap Items?
	if (LocalSwapInventoryItem.Amount > 0)
	{
		// ... Still need to rework on this part ...
		
		// Check if Stackable
		// If Stacks Of Same Item And If Not Null
		// Item Is Same &&  Item Is Stackable 
		// Item Has Free Stack Space
		if (/*LocalToInventoryItem.ItemStructure.EquipmentSlot == LocalSwapInventoryItem.ItemStructure.EquipmentSlot 
			&&*/ LocalSwapInventoryItem.ItemStructure.IsStackable
			&& LocalSwapInventoryItem.ItemStructure.MaxStackSize - LocalSwapInventoryItem.Amount > 0)
		{
			// Add To Stack
			uint8 AmountRemaining = 0;
			AddItemToStack(ToInventory, ToInventorySlot, LocalInventoryItem.Amount, AmountRemaining);

			if (AmountRemaining > 0)
			{
				// Update the From Stack Amount
				//SetItemAmount();
				LocalInventoryItem.Amount = AmountRemaining;

				AddItem2(FromInventory, FromInventorySlot, LocalInventoryItem);
				return;
			}else
			{
				// Full Amount Was Moved Clear Old Stack
				RemoveItem2(FromInventory, FromInventorySlot);
				return;
			}
		}
	}else
	{
		AddItem2(ToInventory, ToInventorySlot, LocalInventoryItem);
		RemoveItem2(FromInventory, FromInventorySlot);
	}
}

void UInventoryManagerComponent::AddItemToStack(UInventoryComponent* Inventory, uint8 InventorySlot, uint8 AmountToAdd,
	uint8& AmountRemaining)
{
	UInventoryComponent* LocalInventory = Inventory;
	uint8 LocalInventorySlot = InventorySlot;
	uint8 LocalAmountToAdd = AmountToAdd;
	uint8 LocalRemainingAmount = AmountToAdd;

	FSlotStructure LocalInventoryItem = LocalInventory->GetInventorySlot(LocalInventorySlot);

	// Does Stack Has Free Space?
	if (LocalInventoryItem.Amount < LocalInventoryItem.ItemStructure.MaxStackSize)
	{
		// Does The Full Amount To Add Fit In Stack?
		// AmountToAdd <= FreeStackSpace
		if (LocalAmountToAdd <= LocalInventoryItem.ItemStructure.MaxStackSize - LocalInventoryItem.Amount)
		{
			LocalRemainingAmount = 0;
			//AddToItemAmount(LocalInventoryItem, AmountToAdd);
			LocalInventoryItem.Amount = LocalInventoryItem.Amount + AmountToAdd;
			AddItem2(Inventory, InventorySlot, LocalInventoryItem);
		}else
		{
			LocalRemainingAmount = LocalAmountToAdd - (LocalInventoryItem.ItemStructure.MaxStackSize - LocalInventoryItem.Amount);
			LocalInventoryItem.Amount = LocalInventoryItem.Amount + AmountToAdd;
			AddItem2(Inventory, InventorySlot, LocalInventoryItem);
		}
	}

	AmountRemaining = LocalRemainingAmount;
	
}

void UInventoryManagerComponent::SetViewersContainerSlot(uint8 ContainerSlot, FSlotStructure& InventoryItem)
{
	TArray<APlayerState*> TempPlayersViewing = IInventoryInterface::Execute_GetPlayersViewing(CurrentContainer);

	for (APlayerState* PlayerState : TempPlayersViewing)
	{
		if (AMyPlayerController* PC = Cast<AMyPlayerController>(PlayerState->GetOwner()))
		{
			PC->InventoryManagerComponent->Client_SetContainerSlotItem(InventoryItem, ContainerSlot);
		}
	}
}

void UInventoryManagerComponent::Client_ClearContainerSlotItem_Implementation(uint8 ContainerSlot)
{
	ClearContainerSlotItem(ContainerSlot);
}

void UInventoryManagerComponent::ClearContainerSlotItem(uint8 ContainerSlot)
{
	USlotLayout* SlotLayout = MainLayoutUI->Container->ContainerSlotsArray[ContainerSlot];
	SlotLayout->SlotStructure = GetEmptySlot(EEquipmentSlot::Undefined);
	SlotLayout->UpdateSlot2();
}

void UInventoryManagerComponent::ClearViewersContainerSlot(uint8 ContainerSlot)
{
	TArray<APlayerState*> LocalPlayersViewing = IInventoryInterface::Execute_GetPlayersViewing(CurrentContainer);
	
	for (APlayerState* PlayerState : LocalPlayersViewing)
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(PlayerState->GetOwner()))
		{
			PlayerController->InventoryManagerComponent->Client_ClearContainerSlotItem(ContainerSlot);
		}
	}
}

void UInventoryManagerComponent::SetContainerSlotItem(const FSlotStructure& Slot, uint8 Index)
{
	USlotLayout* SlotLayout = MainLayoutUI->Container->ContainerSlotsArray[Index];
	SlotLayout->SlotStructure = Slot;
	SlotLayout->UpdateSlot2();
}

void UInventoryManagerComponent::AddItem2(UInventoryComponent* Inventory,
                                          uint8 InventorySlot, FSlotStructure& InventoryItem)
{
	Inventory->SetInventoryItem(InventorySlot, InventoryItem);

	// Are we updating Player's inventory?
	if (PlayerInventory == Inventory)
	{
		//Client: Update HUD Inventory Slot Info
		Client_SetInventorySlotItem(InventoryItem, InventorySlot);
	}else
	{
		//Server: Update HUD Container Slot Info For All Viewers
		SetViewersContainerSlot(InventorySlot, InventoryItem);
	}
}

void UInventoryManagerComponent::RemoveItem2(UInventoryComponent* Inventory, uint8 InventorySlot)
{
	Inventory->ClearInventoryItem(InventorySlot);

	if (PlayerInventory == Inventory)
	{
		Client_ClearInventorySlotItem(InventorySlot);
	}else
	{
		ClearViewersContainerSlot(InventorySlot);	
	}
}

void UInventoryManagerComponent::UseContainer(AActor* Container)
{
	if(Container->Implements<UInventoryInterface>())
	{
		if (CurrentContainer != Container)
		{
			OpenContainer(Container);
		}else
		{
			Server_CloseContainer();
		}
	}
}

void UInventoryManagerComponent::OpenContainer(AActor* Container)
{
	CurrentContainer = Container;
	
	UInventoryComponent* GetContainerInventoryTemp{};
	IInventoryInterface::Execute_GetContainerInventory(CurrentContainer, GetContainerInventoryTemp);
	ContainerInventory = GetContainerInventoryTemp;
	
	TArray<FSlotStructure> LocalInventory{};
	FSlotStructure LocalEmptySlot = GetEmptySlot(EEquipmentSlot::Undefined);
	for (FSlotStructure Slot : ContainerInventory->Inventory)
	{
		if (Slot.Amount == 0)
		{
			LocalInventory.Add(LocalEmptySlot);
		}
		else
		{
			LocalInventory.Add(Slot);
		}
	}

	FName LocalName;
	uint8 LocalNumberOfRows;
	uint8 LocalSlotsPerRow;
	bool LocalIsStorageContainer;
	uint8 LocalInventorySize;

	IInventoryInterface::Execute_GetContainerProperties(Container, LocalName, LocalNumberOfRows, LocalSlotsPerRow,
	                                                    LocalIsStorageContainer, LocalInventorySize);
	FContainerInfo C_Info;
	C_Info.ContainerName = LocalName;
	C_Info.NumberOfRows = LocalNumberOfRows;
	C_Info.SlotsPerRow = LocalSlotsPerRow;
	C_Info.IsStorageContainer = LocalIsStorageContainer;
	C_Info.StorageInventorySize = LocalInventorySize;

	Client_OpenContainer(C_Info, LocalInventory);
}

void UInventoryManagerComponent::CloseContainer()
{
	if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetOwner()))
	{
		if (AContainerActor* CurrentContainerActor = Cast<AContainerActor>(CurrentContainer))
		{
			CurrentContainerActor->PlayersViewing.Remove(PlayerController->PlayerState);
		}
	}

	CurrentContainer = nullptr;
	ContainerInventory = nullptr;
	
	Client_CloseContainer();
}

void UInventoryManagerComponent::LoadContainerSlots(FContainerInfo ContainerProperties,
	const TArray<FSlotStructure>& InContainerInventory)
{
	CreateContainerSlots(ContainerProperties.NumberOfRows, ContainerProperties.SlotsPerRow);
	
	//PC->RefreshContainerUI(ContainerInventory->SlotsPerRowInventory, ContainerInventory->NumberOfRowsInventory);
	
	uint8 Index = 0;
	for (FSlotStructure Slot:InContainerInventory)
	{
		//MainLayoutUI->Container->ContainerSlotsArray[Index]->SlotStructure = Slot;
		//MainLayoutUI->Container->ContainerSlotsArray[Index]->UpdateSlot2();
		//MainLayoutUI->Container->ContainerSlotsArray[Index]->UpdateSlot(Slot);	

		SetContainerSlotItem(Slot, Index);
		Index++;
	}
	
	AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwner());
	PC->ToggleContainer();
}

bool UInventoryManagerComponent::MoveInventoryItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot)
{
	if (FromInventorySlot != ToInventorySlot)
	{
		const FSlotStructure LocalSlot = GetInventorySlot(FromInventorySlot);
		const FSlotStructure SwapSlot = GetInventorySlot(ToInventorySlot);

		SetInventorySlotItem(LocalSlot, ToInventorySlot);
		SetInventorySlotItem(SwapSlot, FromInventorySlot);

		return true;
	}
	return false;
}

FSlotStructure UInventoryManagerComponent::GetInventorySlot(const uint8& InventorySlot)
{
	if (PlayerInventory->Inventory[InventorySlot].Amount > 0)
	{
		return PlayerInventory->Inventory[InventorySlot];
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
		PlayerInventory->Inventory[InventorySlot] = InventoryItem;
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
		UE_LOG(LogTemp, Warning, TEXT("NO FREE SPACE"))
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
		// FSlotStructure Slot = Inventory[Index];
		FSlotStructure Slot = PlayerInventory->Inventory[Index];
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
	// Clear Inventory Item - Set Empty Slot as None
	PlayerInventory->Inventory[InventorySlot] = GetEmptySlot(GetEquipmentTypeBySlot(InventorySlot));
}

void UInventoryManagerComponent::Client_SetInventorySlotItem_Implementation(const FSlotStructure& ContentToAdd, const uint8& InventorySlot)
{
	SetInventorySlotItem(ContentToAdd, InventorySlot);
}

void UInventoryManagerComponent::SetInventorySlotItem(const FSlotStructure& ContentToAdd, const uint8& InventorySlot)
{
	if (IsValid(MainLayoutUI))
	{
		// temporary fix 
		uint8 Index = InventorySlot - (uint8)EEquipmentSlot::Count;
		
		MainLayoutUI->Inventory->InventorySlotsArray[Index]->SlotStructure = ContentToAdd;

		//MainLayoutUI->Inventory->InventorySlotsArray[InventorySlot]->UpdateSlot(ContentToAdd);
		MainLayoutUI->Inventory->InventorySlotsArray[Index]->UpdateSlot2();
	}
}

void UInventoryManagerComponent::ClearInventorySlotItem(uint8 InventorySlot)
{
	if (IsValid(MainLayoutUI))
	{
		// temporary fix 
		uint8 Index = InventorySlot - (uint8)EEquipmentSlot::Count;
		
		USlotLayout* SlotLayout = MainLayoutUI->Inventory->InventorySlotsArray[Index];
		SlotLayout->SlotStructure = GetEmptySlot(EEquipmentSlot::Undefined);
		SlotLayout->UpdateSlot2();
	}
}

void UInventoryManagerComponent::ClearContainerSlots()
{
	MainLayoutUI->Container->ContainerSlotsArray.Empty();
	MainLayoutUI->Container->ContainerGridPanel->ClearChildren();
}

void UInventoryManagerComponent::CreateContainerSlots(uint8 NumberOfRows, uint8 SlotsPerRow)
{
	ClearContainerSlots();

	//UFUNCTION(Category = "UserInterface|Private|Container") AddContainerSlot(Row,Column,Slot,IsStorage);
	AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwner());
	
	FWidgetsLayoutBP* WidgetLayout = Cast<AMyHUD>(PC->HUD_Reference)->GetWidgetBPClass("SlotLayout_WBP");
	if (WidgetLayout)
	{
		USlotLayout* W_ContainerSlot = nullptr;
		
		for(int i = 0; i < NumberOfRows; i++)
		{
			for(int j = 0; j < SlotsPerRow; j++)
			{
				W_ContainerSlot = CreateWidget<USlotLayout>(GetWorld(), WidgetLayout->Widget);
				MainLayoutUI->Container->ContainerGridPanel->AddChildToUniformGrid(W_ContainerSlot, i, j);
				
				MainLayoutUI->Container->ContainerSlotsArray.Add(W_ContainerSlot);
			}
		}
		
		const FSlotStructure SlotStructure = GetEmptySlot(EEquipmentSlot::Undefined);
	
		for(int i = 0; i < MainLayoutUI->Container->ContainerSlotsArray.Num(); i++)
		{
			//MainLayoutUI->Container->ContainerSlotsArray[i]->SlotStructure = SlotStructure;
			//MainLayoutUI->Container->ContainerSlotsArray[i]->UpdateSlot2();
			
			MainLayoutUI->Container->ContainerSlotsArray[i]->UpdateSlot(SlotStructure);
			MainLayoutUI->Container->ContainerSlotsArray[i]->InventorySlotIndex = i;
			MainLayoutUI->Container->ContainerSlotsArray[i]->NativeFromContainer = true;
		
			MainLayoutUI->Container->ContainerSlotsArray[i]->IsStorageSlot = true;
		}
	}
}

void UInventoryManagerComponent::ClearInventoryItem(const uint8& InventorySlot)
{
	PlayerInventory->Inventory[InventorySlot] = GetEmptySlot(GetEquipmentTypeBySlot(InventorySlot));
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

void UInventoryManagerComponent::UpdateEquippedStats()
{
	uint8 TempStrength = 0;
	uint8 TempEndurance = 0;
	uint8 TempDexterity = 0;
	uint8 TempIntelligence = 0;

	
	for (int i = 0; i < (uint8)EEquipmentSlot::Count; i++)
	{
		FSlotStructure TempSlot = PlayerInventory->GetInventorySlot(i);
		
		TempStrength += TempSlot.ItemStructure.Strength;
		TempEndurance += TempSlot.ItemStructure.Endurance;
		TempDexterity += TempSlot.ItemStructure.Dexterity;
		TempIntelligence += TempSlot.ItemStructure.Intelligence;
	}
	
	AttributesArray[0] = TempStrength;
	AttributesArray[1] = TempEndurance;
	AttributesArray[2] = TempDexterity;
	AttributesArray[3] = TempIntelligence;
}

void UInventoryManagerComponent::InitializePlayerAttributes()
{
	AttributesArray.Init(0, (uint8)EAttributes::Count);
	
	UpdateEquippedStats();
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

void UInventoryManagerComponent::Client_OpenContainer_Implementation(FContainerInfo ContainerProperties,
	const TArray<FSlotStructure>& InContainerInventory)
{
	LoadContainerSlots(ContainerProperties, InContainerInventory);
}

void UInventoryManagerComponent::Client_CloseContainer_Implementation()
{
	if (AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwner()))
	{
		PC->ToggleContainer();
	}else
	{
		MainLayoutUI->Container->ToggleWindow();
	}
}

void UInventoryManagerComponent::Server_TakeContainerItem_Implementation(const uint8& FromContainerSlot,
	const uint8& ToInventorySlot)
{
	MoveItem(ContainerInventory, FromContainerSlot, PlayerInventory, ToInventorySlot);
}

void UInventoryManagerComponent::Server_DepositContainerItem_Implementation(const uint8& FromInventorySlot,
	const uint8& ToInventorySlot)
{
	MoveItem(PlayerInventory, FromInventorySlot, ContainerInventory, ToInventorySlot);
}

void UInventoryManagerComponent::Client_SetContainerSlotItem_Implementation(const FSlotStructure& ContentToAdd,
	const uint8& InventorySlot)
{
	SetContainerSlotItem(ContentToAdd, InventorySlot);
}

void UInventoryManagerComponent::Client_ClearInventorySlotItem_Implementation(uint8 InventorySlot)
{
	ClearInventorySlotItem(InventorySlot);
}

void UInventoryManagerComponent::InitializeInventoryManagerUI(UMainLayout* MainLayout)
{
	MainLayoutUI = MainLayout;
}

EEquipmentSlot UInventoryManagerComponent::GetEquipmentTypeBySlot(const uint8& EquipmentSlot)
{
	//return Inventory[EquipmentSlot].ItemStructure.EquipmentSlot;
	return PlayerInventory->Inventory[EquipmentSlot].ItemStructure.EquipmentSlot;
}

EItemType UInventoryManagerComponent::GetItemTypeBySlot(const uint8& ItemSlot)
{
	//return Inventory[ItemSlot].ItemStructure.ItemType;
	return PlayerInventory->Inventory[ItemSlot].ItemStructure.ItemType;
}

void UInventoryManagerComponent::Server_DropItemFromInventory_Implementation(const uint8& InventorySlot)
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

void UInventoryManagerComponent::Server_UseContainer_Implementation(AActor* Container)
{
	UseContainer(Container);
}

void UInventoryManagerComponent::Server_CloseContainer_Implementation()
{
	CloseContainer();
}