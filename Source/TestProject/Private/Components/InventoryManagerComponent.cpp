// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryManagerComponent.h"
#include "MyHUD.h"
#include "MyPlayerController.h"
#include "Components/UniformGridPanel.h"
#include "Inventory/FContainerInfo.h"
#include "Net/UnrealNetwork.h"
#include "UI/ContainerLayout.h"

DECLARE_LOG_CATEGORY_CLASS(LogInventory, Verbose, Verbose);

// Sets default values for this component's properties
UInventoryManagerComponent::UInventoryManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
	
	// Get ItemDB 
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_ItemDB(TEXT("/Game/Blueprints/Item_DB.Item_DB"));
	if (BP_ItemDB.Succeeded())
	{
		ItemDB = BP_ItemDB.Object;
	}else{
	UE_LOG(LogInventory, Warning, TEXT ("ItemDB DataTable not found!!"));
	}
	
	NumberOfSlots = 28 + (uint8)EEquipmentSlot::Count + 9;
	//InventorySize = 28 + (uint8)EEquipmentSlot::Count + 9;;
}

// Called when the game starts
void UInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// Inventory + Equipment + Container
	//NumberOfSlots = 28 + (uint8)EEquipmentSlot::Count + 9;
	//InitInventory(NumberOfSlots);
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

	//DOREPLIFETIME(UInventoryManagerComponent, InventorySize);
	DOREPLIFETIME(UInventoryManagerComponent, NumberOfSlots);
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
	//Inventory.Reserve(NumberSlots);
	PlayerInventory->Inventory.Reserve(NumberSlots);

	FSlotStructure SlotStructure = {};
	//Inventory.Init(SlotStructure, NumberSlots);
	PlayerInventory->Inventory.Init(SlotStructure, NumberSlots);

	// Add Customized Icons to Slots
	uint8 Index = 0;
	//for (FSlotStructure& CurrentSlot : Inventory)
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
	PlayerInventory->Server_InitInventory_Implementation(NumberOfSlots);
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
	for (size_t i = (uint8)EEquipmentSlot::Count; i < NumberOfSlots - 9; i++)
	{
		const FSlotStructure& CurrentSlot = PlayerInventory->Inventory[i]; //Inventory[i];
		if (CurrentSlot.Amount <= 0)
		{
			HasSpace = true;
			IdentifiedIndex = i;
			break;
		}
	}
	
	if (HasSpace)
	{
		PlayerInventory->Inventory[IdentifiedIndex] = ContentToAdd; //Inventory[IdentifiedIndex]; = ContentToAdd;

		/**/
		const uint8 MaxStackSize = ContentToAdd.ItemStructure.MaxStackSize;
		const uint8 FinalQuantity = ContentToAdd.Amount;
	
		if (FinalQuantity > MaxStackSize)
		{
			PlayerInventory->Inventory[IdentifiedIndex].Amount = MaxStackSize; // Inventory[IdentifiedIndex];
		
			const uint8 RestAmountToAdd = (FinalQuantity - MaxStackSize);

			ContentToAdd.Amount = RestAmountToAdd;
		
			AddItemToInventory(ContentToAdd);
		}else
		{
			PlayerInventory->Inventory[IdentifiedIndex].Amount = FinalQuantity;
			//Inventory[IdentifiedIndex].Amount = FinalQuantity;
		}
		/**/
		return true;
	}
	
	return false;
}

bool UInventoryManagerComponent::AddToStack(FSlotStructure& ContentToAdd, const int8& Index)
{
	const FSlotStructure& CurrentSlot = PlayerInventory->Inventory[Index]; //Inventory[Index]
	const uint8 MaxStackSize = CurrentSlot.ItemStructure.MaxStackSize;
	
	const uint8 FinalQuantity = CurrentSlot.Amount + ContentToAdd.Amount;
	
	if (FinalQuantity > MaxStackSize)
	{
		PlayerInventory->Inventory[Index].Amount = MaxStackSize;
		//Inventory[Index].Amount = MaxStackSize;
		
		const uint8 RestAmountToAdd = (FinalQuantity - MaxStackSize);

		ContentToAdd.Amount = RestAmountToAdd;
		
		AddItemToInventory(ContentToAdd);
	}else
	{
		PlayerInventory->Inventory[Index].Amount = FinalQuantity;
		//Inventory[Index].Amount = FinalQuantity;
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
		//const bool SameID = Inventory[i].ItemStructure.ID == ContentToAdd.ItemStructure.ID;
		const bool SameID = PlayerInventory->Inventory[i].ItemStructure.ID == ContentToAdd.ItemStructure.ID;
		
		//const bool InsideStackLimit = Inventory[i].Amount < ContentToAdd.ItemStructure.MaxStackSize;
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
			
			SetInventorySlot(LocalSlot, ToInventorySlot);
			SetInventorySlot(SwapSlot, FromInventorySlot);

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
		
			SetInventorySlot(LocalSlot, ToInventorySlot);
			SetInventorySlot(SwapSlot, FromInventorySlot);

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

void UInventoryManagerComponent::UseContainer(AActor* Container)
{
	if(Container->Implements<UInventoryInterface>())
	{
		if (CurrentContainer != Container)
		{
			OpenContainer(Container);
		}else
		{
			//Server_CloseContainer()
		}
	}
}

void UInventoryManagerComponent::OpenContainer(AActor* Container)
{
	//CurrentContainer = Container;	// Crash on this line on multiplayer

	if (AContainerActor* CurrentContainer2 = Cast<AContainerActor>(Container))
	{
		FName LocalName;
		uint8 LocalSlotsPerRow;
		bool LocalIsStorageContainer;
		uint8 LocalInventorySize;
		
		CurrentContainer2->GetContainerProperties_Implementation(LocalName, LocalSlotsPerRow, LocalIsStorageContainer, LocalInventorySize);
		
		UInventoryComponent* ContainerInventory2 = CurrentContainer2->GetContainerInventory_Implementation();
		
		TArray<FSlotStructure> LocalInventory {};
		//LocalInventory.Init(GetEmptySlot(EEquipmentSlot::Undefined),9);
		
		LocalInventory.Init(GetEmptySlot(EEquipmentSlot::Undefined),CurrentContainer2->C_InventorySize);

		
		//for(uint8 i = 0; i < 9; i++)
		for(uint8 i = 0; i < CurrentContainer2->C_InventorySize; i++)
		{
			//LocalInventory[i] = Inventory[28 + i];	
			LocalInventory[i] = PlayerInventory->Inventory[28 + i];	
		}
		
		/*for (FSlotStructure Slot : ContainerInventory->Inventory)
		{
			LocalInventory->Add(Slot);
		}*/
		
		FContainerInfo C_Info;
		C_Info.ContainerName = LocalName;
		C_Info.SlotsPerRow = LocalSlotsPerRow;
		C_Info.IsStorageContainer = LocalIsStorageContainer;
		C_Info.StorageInventorySize = LocalInventorySize;

		//Client_OpenContainer(C_Info, LocalInventory);

		AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwner());
		PC->RefreshContainerUI(ContainerInventory2->RowsPerSlotInventory, ContainerInventory2->NumberOfRowsInventory);
		PC->RefreshWidgets();
		PC->ToggleContainer();
	}
}

void UInventoryManagerComponent::CloseContainer()
{
}

void UInventoryManagerComponent::LoadContainerSlots(const FContainerInfo& ContainerProperties,
	const TArray<FSlotStructure>& InContainerInventory)
{
	InventoryUI->Container->RefreshWindow();

	//InventoryUI->Container->SetVisibility(ESlateVisibility::Visible);
}

void UInventoryManagerComponent::AddContainerSlot(uint8 Row, uint8 Column, uint8 Slot, bool IsStorage)
{
	FWidgetsLayoutBP* WidgetLayout = Cast<AMyHUD>(GetOwner())->GetWidgetBPClass("SlotLayout_WBP");
	if (WidgetLayout)
	{
		USlotLayout* W_Slot = nullptr;
		
		W_Slot = CreateWidget<USlotLayout>(GetWorld(), WidgetLayout->Widget);

		W_Slot->IsStorageSlot = IsStorage;
		
		InventoryUI->Container->ContainerGridPanel->AddChildToUniformGrid(W_Slot, Row, Column);
		
		InventoryUI->Container->ContainerSlots.Add(W_Slot);
		
	}
}

void UInventoryManagerComponent::CreateContainerSlots(uint8 ContainerSize, uint8 SlotsPerRow)
{
	// Clear Container Slots
	
	if (ContainerSize <= 0)
	{
		return;
	}

	//
	uint8 LocalContainerSize = ContainerSize;
	bool LocalIsStorageContainer = InventoryUI->Container->IsStorageContainer;
	uint8 LocalMaxRowSize = SlotsPerRow;

	float NumberOfRows = FMath::CeilToFloat(LocalContainerSize / LocalMaxRowSize) - 1;
	float LastIndex = FMath::Max(NumberOfRows, 0.f);
	uint8 LocalLoopCount = 0;
	
	for(uint8 i = 0; i < LastIndex; i++)
	{
		for (uint8 j = 0; j < LocalMaxRowSize - 1; j++)
		{
			AddContainerSlot(i, j, LocalLoopCount, LocalIsStorageContainer);
			LocalLoopCount++;

			if (LocalLoopCount == LocalContainerSize)
			{
				return;
			}
		}
	}

}

void UInventoryManagerComponent::SetContainerSlotItem(uint8 ContainerSlot, FSlotStructure SlotInformation)
{
	
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

FSlotStructure UInventoryManagerComponent::GetInventorySlot(const uint8& InventorySlot)
{
	//if (Inventory[InventorySlot].Amount > 0)
	if (PlayerInventory->Inventory[InventorySlot].Amount > 0)
	{
		//return Inventory[InventorySlot];
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
		//Inventory[InventorySlot] = InventoryItem;
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
	// Clear Inventory Item
	// Inventory[InventorySlot] = GetEmptySlot(GetEquipmentTypeBySlot(InventorySlot));
	PlayerInventory->Inventory[InventorySlot] = GetEmptySlot(GetEquipmentTypeBySlot(InventorySlot));
}

void UInventoryManagerComponent::Client_SetInventorySlot_Implementation(const FSlotStructure& ContentToAdd, const uint8& InventorySlot)
{
	SetInventorySlot(ContentToAdd, InventorySlot);
}

void UInventoryManagerComponent::SetInventorySlot(const FSlotStructure& ContentToAdd, const uint8& InventorySlot)
{
	//Inventory[InventorySlot] = ContentToAdd;
	PlayerInventory->Inventory[InventorySlot] = ContentToAdd;

	//InventoryUI->InventorySlots[InventorySlot] = ContentToAdd;
}

void UInventoryManagerComponent::ClearInventorySlot(const uint8& InventorySlot)
{
	//Inventory[InventorySlot] = GetEmptySlot(GetEquipmentTypeBySlot(InventorySlot));
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
		//FSlotStructure TempSlot = GetInventorySlot(i);
		
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

void UInventoryManagerComponent::Client_OpenContainer_Implementation(const FContainerInfo& ContainerProperties,
	const TArray<FSlotStructure>& InContainerInventory)
{
	LoadContainerSlots(ContainerProperties, InContainerInventory);
}

void UInventoryManagerComponent::Server_Take_ContainerItem_Implementation(const uint8& FromInventorySlot,
	const uint8& ToInventorySlot)
{
	MoveInventoryItem(FromInventorySlot, ToInventorySlot);
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