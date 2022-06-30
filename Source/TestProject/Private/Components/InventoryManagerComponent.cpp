// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryManagerComponent.h"

#include "ContainerActor.h"
#include "EAttributes.h"
#include "FWidgetsLayoutBP.h"
#include "MyHUD.h"
#include "WorldActor.h"
#include "Components/EquipmentComponent.h"
#include "Components/UniformGridPanel.h"
#include "UI/ContainerLayout.h"
#include "UI/InventoryLayout.h"
#include "UI/ProfileLayout.h"
#include "GameFramework/PlayerState.h"
#include "Inventory/FContainerInfo.h"
#include "Net/UnrealNetwork.h"
#include "UI/MainLayout.h"

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
	DOREPLIFETIME(UInventoryManagerComponent, AttributesArray);
}

void UInventoryManagerComponent::Server_InitInventory_Implementation()
{
	PlayerInventory->Server_InitInventory(NumberOfSlots);
}

void UInventoryManagerComponent::InitializeInventoryManager(UInventoryComponent* EquipmentComponent)
{
	PlayerInventory = EquipmentComponent;
}

void UInventoryManagerComponent::FindAndAddAmountToStacks(UInventoryComponent* Inventory, FName ItemID, uint8 Amount, uint8& AmountRemaining)
{
	uint8 LocalAmount = Amount;
	FName LocalItemID = ItemID;
	UInventoryComponent* LocalInventory = Inventory;

	uint8 LocalInventorySlot = 0;
	uint8 InitialIndex = 0;
	if (LocalInventory == PlayerInventory)
	{
		InitialIndex = (uint8)EEquipmentSlot::Count;
		LocalInventorySlot = (uint8)EEquipmentSlot::Count;
	}
	
	for (uint8 i = InitialIndex; i < LocalInventory->Inventory.Num(); i++)
	{
		if (LocalItemID == LocalInventory->Inventory[LocalInventorySlot].ItemStructure.ID)
		{
			AddItemToStack(LocalInventory, LocalInventorySlot, LocalAmount, AmountRemaining);
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

void UInventoryManagerComponent::TryToAddItemToInventory(UInventoryComponent* Inventory, FSlotStructure& InventoryItem,
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

	FReturnTupleBoolInt LocalTuple;
	if (LocalInventory == PlayerInventory)
	{
		LocalTuple = LocalInventory->GetEmptyInventorySpace();
	}
	else if	(LocalInventory == ContainerInventory)
	{
		LocalTuple = LocalInventory->GetEmptyContainerSpace();
	}

	if (LocalTuple.Success)
	{
		AddItem(LocalInventory, LocalTuple.Index, LocalInventoryItem);
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

void UInventoryManagerComponent::Server_MoveContainerItem_Implementation(uint8 FromInventorySlot, uint8 ToInventorySlot)
{
	MoveItem(ContainerInventory, FromInventorySlot, ContainerInventory, ToInventorySlot);
}

UDataTable* UInventoryManagerComponent::GetItemDB()
{
	return ItemDB;
}

EEquipmentSlot UInventoryManagerComponent::GetItemEquipmentSlot(FSlotStructure LocalInventoryItem)
{
	return LocalInventoryItem.ItemStructure.EquipmentSlot;
}

void UInventoryManagerComponent::EquipItem(UInventoryComponent* FromInventory, uint8 FromInventorySlot,
                                           UInventoryComponent* ToInventory, uint8 ToInventorySlot)
{
	if (FromInventory == ToInventory && FromInventorySlot == ToInventorySlot)
	{
		return;
	}
	
	if (GetItemTypeBySlot(FromInventorySlot) == EItemType::Equipment)
	{
		FSlotStructure LocalInventoryItem = FromInventory->GetInventorySlot(FromInventorySlot);
		EEquipmentSlot LocalEquipmentSlotType = GetItemEquipmentSlot(LocalInventoryItem);
		
		if (GetEquipmentTypeBySlot(ToInventorySlot) == LocalEquipmentSlotType)
		{
			FSlotStructure LocalSwapInventoryItem = ToInventory->GetInventorySlot(ToInventorySlot);

			// Swap Items
			if (ItemIsValid(LocalSwapInventoryItem))
			{
				// ...
				AddItem(ToInventory, ToInventorySlot, LocalInventoryItem);
				AddItem(FromInventory, FromInventorySlot, LocalSwapInventoryItem);
			}else
			{
				AddItem(ToInventory, ToInventorySlot, LocalInventoryItem);
				RemoveItem(FromInventory, FromInventorySlot);
			}

			UpdateEquippedStats();
			
			return;
			
		}else{
			UE_LOG(LogTemp, Warning, TEXT("ITEM CAN NOT BE EQUIPPED IN THAT SLOT"))
		}
	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("ITEM IS NOT EQUIPPABLE"))
	}
}

void UInventoryManagerComponent::UnEquipItem(UInventoryComponent* FromInventory, uint8 FromInventorySlot,
                                             UInventoryComponent* ToInventory, uint8 ToInventorySlot)
{
	if (FromInventory == ToInventory && FromInventorySlot == ToInventorySlot)
	{
		return;
	}

	//Is The Item Equippable?
	//Does The Item Have The Same Equip Slot?
	FSlotStructure LocalInventoryItem = FromInventory->GetInventorySlot(FromInventorySlot);
	FSlotStructure LocalSwapInventoryItem = ToInventory->GetInventorySlot(ToInventorySlot);
	EEquipmentSlot LocalEquipmentSlot = GetEquipmentTypeBySlot(FromInventorySlot);
	
	// Swap Items
	if (ItemIsValid(LocalSwapInventoryItem))
	{
		if (GetItemTypeBySlot(FromInventorySlot) != EItemType::Equipment)
		{
			UE_LOG(LogTemp, Warning, TEXT("ITEM IS NOT EQUIPPABLE"))
			return;
		}

		if (GetEquipmentTypeBySlot(ToInventorySlot) != LocalEquipmentSlot)
		{
			UE_LOG(LogTemp, Warning, TEXT("ITEM CAN NOT BE EQUIPPED IN THAT SLOT"))
			return;
		}

		AddItem(ToInventory, ToInventorySlot, LocalInventoryItem);
		AddItem(FromInventory, FromInventorySlot, LocalSwapInventoryItem);
	}
	else
	{
		if (Cast<UEquipmentComponent>(ToInventory))
		{
			if (ToInventorySlot < (uint8)EEquipmentSlot::Count)
			{
				if (GetEquipmentTypeBySlot(ToInventorySlot) != LocalEquipmentSlot)
				{
					UE_LOG(LogTemp, Warning, TEXT("ITEM CAN NOT BE EQUIPPED IN THAT SLOT"))
					return;
				}
			}
		}
		
		AddItem(ToInventory, ToInventorySlot, LocalInventoryItem);
		RemoveItem(FromInventory, FromInventorySlot);
	}

	UpdateEquippedStats();
}

void UInventoryManagerComponent::RandomizeDropLocation(FSlotStructure LocalSlot, UClass*& LocalClass, FTransform& OutTransform)
{
	LocalClass = LocalSlot.ItemStructure.Class;

	// Drop at character feet
	FVector LocalLocation {0.0f, 0.0f, -98.0f};
			
	FVector PawnLocation = Cast<AController>(GetOwner())->GetPawn()->GetActorLocation();

	//Drop Distance Range From Character
	//const uint8 DropDistanceRangeX = FMath::RandRange(64, 96);
	const uint8 DropDistanceRangeX = FMath::RandRange(75, 100);
	FVector DistanceFromPawn {(float)DropDistanceRangeX,1.0f,1.0f};

	// Drop Items 360 Degrees Around Player
	const float DropItemsRotation = FMath::FRandRange(-180, 180);
	FRotator Rotation {1.0f, DropItemsRotation, DropItemsRotation}; // Drop Around Player
	//FRotator Rotation {1.0f, 1.0f, DropItemsRotation}; // Drop In One Point

	FVector VectorRotated = Rotation.RotateVector(DistanceFromPawn);

	FVector FinalLocation = PawnLocation + LocalLocation + VectorRotated; 

	// Give The Dropped Object A Random Rotation
	// const int8 RandomRotation = FMath::RandRange(-10, 10);
	// FRotator FinalRotator {0.0f, 0.0f, (float)RandomRotation * 10};
	FRotator FinalRotator {1.0f, 1.0f, 1.0f};

	FVector FinalScale {1.0f,1.0f,1.0f};

	OutTransform = {};
	OutTransform = FTransform(FinalRotator, FinalLocation, FinalScale);
}

void UInventoryManagerComponent::DropItem(UInventoryComponent* Inventory, uint8 InventorySlot)
{
	FSlotStructure LocalSlot = Inventory->GetInventoryItem(InventorySlot);
	
	if (LocalSlot.ItemStructure.IsDroppable)
	{
		UE_LOG(LogTemp, Warning, TEXT("DROPPED ITEM"))
		
		UClass* LocalClass;
		FTransform OutTransform;
		RandomizeDropLocation(LocalSlot, LocalClass, OutTransform);

		// Spawn World Actor
		AWorldActor* WActor = GetWorld()->SpawnActor<AWorldActor>(LocalClass, OutTransform);
		if (WActor)
		{
			WActor->Amount = LocalSlot.Amount;
		}
		/**/
		RemoveItem(Inventory, InventorySlot);

		// Are we dropping an equipped item?
		if (InventorySlot < (uint8)EEquipmentSlot::Count)
		{
			UpdateEquippedStats();
		}
	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("You cannot drop this..."))
		UE_LOG(LogTemp, Warning, TEXT("DESTROYED ITEM"))

		RemoveItem(Inventory, InventorySlot);
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

	FSlotStructure LocalInventoryItem = FromInventory->GetInventoryItem(FromInventorySlot);
	FSlotStructure LocalSwapInventoryItem = ToInventory->GetInventoryItem(ToInventorySlot);

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

				AddItem(FromInventory, FromInventorySlot, LocalInventoryItem);
				return;
			}else
			{
				// Full Amount Was Moved Clear Old Stack
				RemoveItem(FromInventory, FromInventorySlot);
				return;
			}
		}
	}else
	{
		AddItem(ToInventory, ToInventorySlot, LocalInventoryItem);
		RemoveItem(FromInventory, FromInventorySlot);
	}
}

void UInventoryManagerComponent::AddItemToStack(UInventoryComponent* Inventory, uint8 InventorySlot, uint8 AmountToAdd,
	uint8& AmountRemaining)
{
	UInventoryComponent* LocalInventory = Inventory;
	uint8 LocalInventorySlot = InventorySlot;
	uint8 LocalAmountToAdd = AmountToAdd;
	uint8 LocalRemainingAmount = AmountToAdd;

	FSlotStructure LocalInventoryItem = LocalInventory->GetInventoryItem(LocalInventorySlot);

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
			AddItem(Inventory, InventorySlot, LocalInventoryItem);
		}else
		{
			LocalRemainingAmount = LocalAmountToAdd - (LocalInventoryItem.ItemStructure.MaxStackSize - LocalInventoryItem.Amount);
			LocalInventoryItem.Amount = LocalInventoryItem.Amount + AmountToAdd;
			AddItem(Inventory, InventorySlot, LocalInventoryItem);
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

void UInventoryManagerComponent::AddItem(UInventoryComponent* Inventory,
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

void UInventoryManagerComponent::RemoveItem(UInventoryComponent* Inventory, uint8 InventorySlot)
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
	
	uint8 Index = 0;
	for (FSlotStructure Slot:InContainerInventory)
	{
		SetContainerSlotItem(Slot, Index);
		Index++;
	}

	if (AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwner()))
	{
		PC->ToggleContainer();
	}
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
		FSlotStructure LocalSlot = PlayerInventory->GetInventorySlot(Index);

		// Is Same Type And Empty?
		if (!ItemIsValid(LocalSlot) && EquipmentSlot == LocalEquipSlot)
		{
			OutIndex = Index;
			return true;
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

void UInventoryManagerComponent::Client_SetInventorySlotItem_Implementation(const FSlotStructure& ContentToAdd, const uint8& InventorySlot)
{
	SetInventorySlotItem(ContentToAdd, InventorySlot);
}

void UInventoryManagerComponent::SetInventorySlotItem(const FSlotStructure& ContentToAdd, const uint8& InventorySlot)
{
	if (IsValid(MainLayoutUI))
	{
		if (InventorySlot >= (uint8)EEquipmentSlot::Count)
		{
			uint8 LocalIndex = InventorySlot - (uint8)EEquipmentSlot::Count;
			MainLayoutUI->Inventory->InventorySlotsArray[LocalIndex]->SlotStructure = ContentToAdd;
			MainLayoutUI->Inventory->InventorySlotsArray[LocalIndex]->UpdateSlot2();
		}else
		{
			MainLayoutUI->Profile->EquipmentSlotsArray[InventorySlot]->SlotStructure = ContentToAdd;
			MainLayoutUI->Profile->EquipmentSlotsArray[InventorySlot]->UpdateSlot2();
		}
	}
}

void UInventoryManagerComponent::ClearInventorySlotItem(uint8 InventorySlot)
{
	if (IsValid(MainLayoutUI))
	{
		USlotLayout* SlotLayout;
		if (InventorySlot >= (uint8)EEquipmentSlot::Count)
		{
			uint8 LocalIndex = InventorySlot - (uint8)EEquipmentSlot::Count;
			
			SlotLayout = MainLayoutUI->Inventory->InventorySlotsArray[LocalIndex];
			SlotLayout->SlotStructure = GetEmptySlot(EEquipmentSlot::Undefined);
			SlotLayout->UpdateSlot2();
		}else
		{
			SlotLayout = MainLayoutUI->Profile->EquipmentSlotsArray[InventorySlot];
			
			// SlotLayout->SlotStructure = GetEmptySlot(GetEquipmentTypeBySlot(InventorySlot));
			FSlotStructure LocalSlot;
			switch (InventorySlot)
			{
			case 0: LocalSlot = GetEmptySlot(EEquipmentSlot::Weapon);
				break;
			case 1: LocalSlot = GetEmptySlot(EEquipmentSlot::Chest);
				break;
			case 2: LocalSlot = GetEmptySlot(EEquipmentSlot::Feet);
				break;
			case 3: LocalSlot = GetEmptySlot(EEquipmentSlot::Hands);
				break;
			default: LocalSlot = GetEmptySlot(EEquipmentSlot::Undefined);
				break;
			}
			
			SlotLayout->SlotStructure = LocalSlot;
			SlotLayout->UpdateSlot2();
		}
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

void UInventoryManagerComponent::UpdateEquippedStats()
{
	uint8 TempStrength = 0;
	uint8 TempEndurance = 0;
	uint8 TempDexterity = 0;
	uint8 TempIntelligence = 0;
	
	for (int i = 0; i < (uint8)EEquipmentSlot::Count; i++)
	{
		FSlotStructure TempSlot = PlayerInventory->GetInventoryItem(i);
		
		TempStrength += TempSlot.ItemStructure.Strength;
		TempEndurance += TempSlot.ItemStructure.Endurance;
		TempDexterity += TempSlot.ItemStructure.Dexterity;
		TempIntelligence += TempSlot.ItemStructure.Intelligence;
	}
	
	AttributesArray[0] = TempStrength;
	AttributesArray[1] = TempEndurance;
	AttributesArray[2] = TempDexterity;
	AttributesArray[3] = TempIntelligence;

	Client_SetAttributes(AttributesArray);
}

void UInventoryManagerComponent::Client_SetAttributes_Implementation(const TArray<uint8>& InAttributesArray)
{
	SetAttributes(InAttributesArray);
}

void UInventoryManagerComponent::SetAttributes(const TArray<uint8>& InAttributesArray)
{
	if (IsValid(MainLayoutUI))
	{
		MainLayoutUI->Profile->UpdatePlayerStatsUI(InAttributesArray);
	}
}

void UInventoryManagerComponent::InitializePlayerAttributes()
{
	AttributesArray.Init(0, (uint8)EAttributes::Count);
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
	MainLayoutUI->Container->SetVisibility(ESlateVisibility::Hidden);
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

void UInventoryManagerComponent::Server_EquipFromContainer_Implementation(uint8 FromInventorySlot, uint8 ToInventorySlot)
{
	EquipItem(ContainerInventory, FromInventorySlot, PlayerInventory, ToInventorySlot);
}

void UInventoryManagerComponent::Server_UnEquipToContainer_Implementation(uint8 FromInventorySlot, uint8 ToInventorySlot)
{
	UnEquipItem(PlayerInventory, FromInventorySlot, ContainerInventory, ToInventorySlot);
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

EItemType UInventoryManagerComponent::GetItemTypeBySlot(uint8 ItemSlot)
{
	return PlayerInventory->Inventory[ItemSlot].ItemStructure.ItemType;
}

void UInventoryManagerComponent::Server_DropItemFromInventory_Implementation(const uint8& InventorySlot)
{
	DropItem(PlayerInventory, InventorySlot);
}

void UInventoryManagerComponent::Server_EquipFromInventory_Implementation(uint8 FromInventorySlot, uint8 ToInventorySlot)
{
	EquipItem(PlayerInventory, FromInventorySlot, PlayerInventory, ToInventorySlot);
}

void UInventoryManagerComponent::Server_UnEquipToInventory_Implementation(uint8 FromInventorySlot, uint8 ToInventorySlot)
{
	UnEquipItem(PlayerInventory, FromInventorySlot, PlayerInventory, ToInventorySlot);
}

void UInventoryManagerComponent::Server_UseContainer_Implementation(AActor* Container)
{
	UseContainer(Container);
}

void UInventoryManagerComponent::Server_CloseContainer_Implementation()
{
	CloseContainer();
}

void UInventoryManagerComponent::Server_UseInventoryItem_Implementation(const uint8& InventorySlot)
{
	UseInventoryItem(InventorySlot);
}

void UInventoryManagerComponent::Server_UseContainerItem_Implementation(const uint8& InventorySlot)
{
	UseContainerItem(InventorySlot);
}

void UInventoryManagerComponent::UseInventoryItem(const uint8& InventorySlot)
{
	FSlotStructure LocalInventorySlot = PlayerInventory->GetInventoryItem(InventorySlot);

	if (IsValid(CurrentContainer))
	{
		switch (LocalInventorySlot.ItemStructure.ItemType)
		{
		case EItemType::Equipment:
			// Are We Unequipping to a container?
			if (InventorySlot < (uint8)EEquipmentSlot::Count)
			{
				UseEquipmentItem(InventorySlot, LocalInventorySlot, ContainerInventory);
				break;
			}
		case EItemType::Consumable:
		default:
			bool bOutSuccess = false;
			TryToAddItemToInventory(ContainerInventory, LocalInventorySlot, bOutSuccess);

			if (bOutSuccess)
			{
				RemoveItem(PlayerInventory, InventorySlot);
			}else
			{
				AddItem(PlayerInventory, InventorySlot, LocalInventorySlot);
			}
			break;
		}
	}else
	{
		switch (LocalInventorySlot.ItemStructure.ItemType)
		{
		case EItemType::Consumable:
			UseConsumableItem(InventorySlot, LocalInventorySlot);
			break;
		case EItemType::Equipment:
			UseEquipmentItem(InventorySlot, LocalInventorySlot, PlayerInventory);
			break;
		default:
			break;
		}
	}
}

void UInventoryManagerComponent::UseContainerItem(const uint8& InventorySlot)
{
	FSlotStructure LocalInventoryItem = ContainerInventory->GetInventoryItem(InventorySlot);

	bool bOutSuccess = false;
	TryToAddItemToInventory(PlayerInventory, LocalInventoryItem, bOutSuccess);

	if (bOutSuccess)
	{
		RemoveItem(ContainerInventory, InventorySlot);
	}else
	{
		AddItem(ContainerInventory, InventorySlot, LocalInventoryItem);
	}
}

void UInventoryManagerComponent::UseConsumableItem(uint8 InventorySlot, FSlotStructure InventoryItem)
{
	// Do something depending on the item properties if needed
	// ...
	if (Cast<AMyPlayerController>(GetOwner()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Consuming this Item...")));
		UE_LOG(LogTemp, Warning, TEXT("Consuming this Item..."))
	}

	// Remove from Item Amount
	uint8 AmountToRemove = 1;
	bool WasFullAmountRemoved = false;
	uint8 AmountRemoved = 0;
	
	RemoveFromItemAmount(InventoryItem, AmountToRemove, WasFullAmountRemoved, AmountRemoved);

	if (WasFullAmountRemoved)
	{
		InventoryItem = GetEmptySlot(EEquipmentSlot::Undefined);

		RemoveItem(PlayerInventory, InventorySlot);
	}else
	{
		AddItem(PlayerInventory, InventorySlot, InventoryItem);
	}
}

void UInventoryManagerComponent::UseEquipmentItem(uint8 InventorySlot, FSlotStructure InventoryItem, UInventoryComponent* ToInventory)
{
	// Are we Unequipping?
	if (InventorySlot < (uint8)EEquipmentSlot::Count)
	{
		uint8 Index = 0;
		if (ToInventory == ContainerInventory)
		{
			FReturnTupleBoolInt LocalTuple = ToInventory->GetEmptyContainerSpace();
			Index = LocalTuple.Index;
			if (LocalTuple.Success)
			{
				UnEquipItem(PlayerInventory, InventorySlot, ToInventory, Index);
				return;	
			}
		}else if (ToInventory == PlayerInventory)
		{
			if (ToInventory->GetEmptyInventorySpace(Index))
			{
				UnEquipItem(PlayerInventory, InventorySlot, ToInventory, Index);
				return;	
			}
		}
		
		UE_LOG(LogTemp, Warning, TEXT("NO FREE SPACE"))
	}
	else
	{
		EEquipmentSlot LocalEquipmentSlot = GetItemEquipmentSlot(InventoryItem);

		uint8 OutInventorySlot = 0;
		// Finds Empty Slot Of Type To Equip 
		if( GetEmptyEquipmentSlotByType(LocalEquipmentSlot, OutInventorySlot) )
		{
			Server_EquipFromInventory(InventorySlot, OutInventorySlot);
		}else
		{
			OutInventorySlot = GetEquipmentSlotByType(LocalEquipmentSlot);
			Server_EquipFromInventory(InventorySlot, OutInventorySlot);
		}
	}
}

//
// bool UInventoryManagerComponent::AddItem(FName ID, uint8 Amount)
// {
// 	const UDataTable* ItemTable = ItemDB;
// 	FItemStructure* NewItemData = ItemTable->FindRow<FItemStructure>(FName(ID), "", true);
//
// 	if (!NewItemData)
// 	{
// 		return false;
// 	}
//
// 	FSlotStructure NewSlot = {};
// 	NewSlot.InitSlot(*NewItemData, Amount);
// 	
// 	return AddItemToInventory(NewSlot);
// }
//
// bool UInventoryManagerComponent::AddItemToInventory(FSlotStructure& ContentToAdd)
// {
// 	if (ContentToAdd.ItemStructure.IsStackable)
// 	{
// 		UE_LOG(LogTemp, Log, TEXT("Item exists on Inventory and it's stackable"));
//
// 		const FReturnTupleBoolInt ReturnValue = HasPartialStack(ContentToAdd);
// 		if (ReturnValue.Success)
// 		{
// 			return AddToStack(ContentToAdd, ReturnValue.Index);
// 		}
// 	}
//
// 	// UE_LOG(LogInventory, Display, TEXT("Item doesn't exists on Inventory or it's not stackable"));
// 	
// 	if (CreateStack(ContentToAdd))
// 	{
// 		return true;
// 	}
// 	
// 	return false;
// }
//
// bool UInventoryManagerComponent::CreateStack(FSlotStructure& ContentToAdd)
// {	
// 	bool HasSpace = false;
// 	uint8 IdentifiedIndex = 0;
//
// 	for (size_t i = (uint8)EEquipmentSlot::Count; i < NumberOfSlots; i++)
// 	{
// 		const FSlotStructure& CurrentSlot = PlayerInventory->Inventory[i];
// 		if (CurrentSlot.Amount <= 0)
// 		{
// 			HasSpace = true;
// 			IdentifiedIndex = i;
// 			break;
// 		}
// 	}
// 	
// 	if (HasSpace)
// 	{
// 		PlayerInventory->Inventory[IdentifiedIndex] = ContentToAdd;
//
// 		/**/
// 		const uint8 MaxStackSize = ContentToAdd.ItemStructure.MaxStackSize;
// 		const uint8 FinalQuantity = ContentToAdd.Amount;
// 	
// 		if (FinalQuantity > MaxStackSize)
// 		{
// 			PlayerInventory->Inventory[IdentifiedIndex].Amount = MaxStackSize;
// 		
// 			const uint8 RestAmountToAdd = (FinalQuantity - MaxStackSize);
//
// 			ContentToAdd.Amount = RestAmountToAdd;
// 		
// 			AddItemToInventory(ContentToAdd);
// 		}else
// 		{
// 			PlayerInventory->Inventory[IdentifiedIndex].Amount = FinalQuantity;
// 		}
// 		/**/
// 		return true;
// 	}
// 	
// 	return false;
// }
//
// bool UInventoryManagerComponent::AddToStack(FSlotStructure& ContentToAdd, const int8& Index)
// {
// 	const FSlotStructure& CurrentSlot = PlayerInventory->Inventory[Index];
// 	const uint8 MaxStackSize = CurrentSlot.ItemStructure.MaxStackSize;
// 	
// 	const uint8 FinalQuantity = CurrentSlot.Amount + ContentToAdd.Amount;
// 	
// 	if (FinalQuantity > MaxStackSize)
// 	{
// 		PlayerInventory->Inventory[Index].Amount = MaxStackSize;
// 		
// 		const uint8 RestAmountToAdd = (FinalQuantity - MaxStackSize);
//
// 		ContentToAdd.Amount = RestAmountToAdd;
// 		
// 		AddItemToInventory(ContentToAdd);
// 	}else
// 	{
// 		PlayerInventory->Inventory[Index].Amount = FinalQuantity;
// 	}
// 	
// 	return true;
// }
//
// FReturnTupleBoolInt UInventoryManagerComponent::HasPartialStack(const FSlotStructure& ContentToAdd)
// {
// 	int8 LocalInteger = -1;
// 	bool LocalBoolean = false;
//
// 	for (size_t i = (uint8)EEquipmentSlot::Count; i < NumberOfSlots; i++)
// 	{
// 		const bool SameID = PlayerInventory->Inventory[i].ItemStructure.ID == ContentToAdd.ItemStructure.ID;
// 		
// 		const bool InsideStackLimit = PlayerInventory->Inventory[i].Amount < ContentToAdd.ItemStructure.MaxStackSize;
// 		
// 		if (SameID && InsideStackLimit)
// 		{
// 			LocalInteger = i;
// 			LocalBoolean = true;
// 			break;
// 		}
// 	}
//
// 	if (LocalBoolean)
// 	{
// 		return {true, LocalInteger};
// 	}
// 	return {false, 0};
// }
// void UInventoryManagerComponent::RemoveItem(const uint8& InventorySlot)
// {
// 	// Clear Inventory Item - Set Empty Slot as None
// 	PlayerInventory->Inventory[InventorySlot] = GetEmptySlot(GetEquipmentTypeBySlot(InventorySlot));
// }