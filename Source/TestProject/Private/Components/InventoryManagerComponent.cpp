// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryManagerComponent.h"

#include "ContainerActor.h"
#include "EAttributes.h"
#include "FWidgetsLayoutBP.h"
#include "MyHUD.h"
#include "WorldActor.h"
#include "Components/EquipmentComponent.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "UI/MainLayout.h"
#include "UI/ContainerLayout.h"
#include "UI/InventoryLayout.h"
#include "UI/ProfileLayout.h"
#include "UI/Hotbar.h"
#include "UI/W_ItemTooltip.h"
#include "UI/SlotLayout.h"
#include "UI/Hotbar_Slot.h"
#include "GameFramework/PlayerState.h"
#include "Inventory/FContainerInfo.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInventoryManagerComponent::UInventoryManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	UDataTable* BP_ItemDB = LoadObject<UDataTable>(this, TEXT("/Game/Blueprints/Item_DB.Item_DB"));
	if (IsValid(BP_ItemDB))
	{
		ItemDB = BP_ItemDB;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UDataTable not Loaded"))
	}

	TotalNumberOfSlots = (NumberOfRowsInventory * SlotsPerRowInventory) + (uint8)EEquipmentSlot::Count;
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

	DOREPLIFETIME(UInventoryManagerComponent, TotalNumberOfSlots);
	DOREPLIFETIME(UInventoryManagerComponent, AttributesArray);
}

void UInventoryManagerComponent::Server_InitInventory_Implementation()
{
	PlayerInventory->Server_InitInventory(TotalNumberOfSlots);
}

void UInventoryManagerComponent::InitializeInventoryManager(UInventoryComponent* EquipmentComponent)
{
	PlayerInventory = EquipmentComponent;
}

void UInventoryManagerComponent::Client_LoadInventoryUI_Implementation()
{
	AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwner());

	FWidgetsLayoutBP* WidgetLayout = Cast<AMyHUD>(PC->HUD_Reference)->GetWidgetBPClass("SlotLayout_WBP");
	if (WidgetLayout)
	{
		USlotLayout* W_Slot = nullptr;

		for (int i = 0; i < NumberOfRowsInventory; i++)
		{
			for (int j = 0; j < SlotsPerRowInventory; j++)
			{
				W_Slot = CreateWidget<USlotLayout>(GetWorld(), WidgetLayout->Widget);
				MainLayoutUI->Inventory->InventoryGridPanel->AddChildToUniformGrid(W_Slot, i, j);

				MainLayoutUI->Inventory->InventorySlotsArray.Add(W_Slot);
			}
		}

		FSlotStructure SlotStructure = GetEmptySlot(EEquipmentSlot::Undefined);

		USlotLayout* LocalSlot{};
		for (int i = 0; i < MainLayoutUI->Inventory->InventorySlotsArray.Num(); i++)
		{
			LocalSlot = MainLayoutUI->Inventory->InventorySlotsArray[i];

			LocalSlot->SetSlotIndex(i + (uint8)EEquipmentSlot::Count);
			LocalSlot->NativeFromInventory = true;

			LocalSlot->UpdateSlot(SlotStructure);
		}
	}
}

void UInventoryManagerComponent::Client_LoadProfileUI_Implementation()
{
	AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwner());

	FWidgetsLayoutBP* WidgetLayout = Cast<AMyHUD>(PC->HUD_Reference)->GetWidgetBPClass("SlotLayout_WBP");
	if (WidgetLayout)
	{
		USlotLayout* W_Slot = nullptr;

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				W_Slot = CreateWidget<USlotLayout>(GetWorld(), WidgetLayout->Widget);

				//MainLayoutUI->Profile->EquipmentGridPanel->AddChildToUniformGrid(W_Slot, i, j);
				UUniformGridSlot* GridSlot = MainLayoutUI->Profile->EquipmentGridPanel->AddChildToUniformGrid(W_Slot, i, j);
				GridSlot->SetHorizontalAlignment(HAlign_Center);
				GridSlot->SetVerticalAlignment(VAlign_Center);

				MainLayoutUI->Profile->EquipmentSlotsArray.Add(W_Slot);
			}
		}

		FSlotStructure SlotStructure{};
		USlotLayout* LocalSlot{};

		for (int i = 0; i < MainLayoutUI->Profile->EquipmentSlotsArray.Num(); i++)
		{
			if (i == 0)
			{
				SlotStructure = GetEmptySlot(EEquipmentSlot::Weapon);
			}
			else if (i == 1)
			{
				SlotStructure = GetEmptySlot(EEquipmentSlot::Chest);
			}
			else if (i == 2)
			{
				SlotStructure = GetEmptySlot(EEquipmentSlot::Feet);
			}
			else if (i == 3)
			{
				SlotStructure = GetEmptySlot(EEquipmentSlot::Hands);
			}
			else {
				SlotStructure = GetEmptySlot(EEquipmentSlot::Undefined);
			}

			LocalSlot = MainLayoutUI->Profile->EquipmentSlotsArray[i];

			LocalSlot->SetSlotIndex(i);
			LocalSlot->NativeFromInventory = true;

			LocalSlot->UpdateSlot(SlotStructure);
		}
	}
}

void UInventoryManagerComponent::Client_LoadHotbarUI_Implementation() {
	LoadHotbarUI();
}

void UInventoryManagerComponent::LoadHotbarUI() {
	AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwner());

	FWidgetsLayoutBP* WidgetLayout = Cast<AMyHUD>(PC->HUD_Reference)->GetWidgetBPClass("HotbarSlot_WBP");
	if (WidgetLayout)
	{
		UHotbar_Slot* W_Slot = nullptr;
		uint8 Row = 0;
		uint8 Column = 0;

		uint8 KeyNumber = 0;
		for (uint8 i = 0; i < NumberOfSlotsOnHotbar; i++)
		{
			Column = i;

			W_Slot = CreateWidget<UHotbar_Slot>(GetWorld(), WidgetLayout->Widget);

			KeyNumber = i + 1;
			if (KeyNumber == PC->GetMaximumHotbarSlots())
			{
				KeyNumber = 0;
			}

			W_Slot->SetKeyNumber(KeyNumber);

			MainLayoutUI->Hotbar->HotbarGridPanel->AddChildToUniformGrid(W_Slot, Row, Column);
			MainLayoutUI->Hotbar->HotbarSlotsArray.Add(W_Slot);
		}

		FSlotStructure SlotStructure = GetEmptySlot(EEquipmentSlot::Undefined);

		UHotbar_Slot* LocalSlot{};
		for (int i = 0; i < MainLayoutUI->Hotbar->HotbarSlotsArray.Num(); i++)
		{
			LocalSlot = MainLayoutUI->Hotbar->HotbarSlotsArray[i];

			LocalSlot->SetSlotIndex(i);
			LocalSlot->NativeFromHotbar = true;

			LocalSlot->UpdateSlot(SlotStructure);
		}
	}
}

void UInventoryManagerComponent::Server_UpdateTooltips_Implementation()
{
	if (IsValid(ContainerInventory))
	{
		Client_UpdateContainerTooltips(PlayerInventory->Inventory, ContainerInventory->Inventory);
	}

	Client_UpdateInventoryTooltips(PlayerInventory->Inventory, PlayerInventory->Inventory);
}

void UInventoryManagerComponent::Client_UpdateContainerTooltips_Implementation(const TArray<FSlotStructure>& InPlayerInventory, const TArray<FSlotStructure>& InOtherInventory)
{
	AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwner());
	if (!IsValid(PC))
	{
		return;
	}
	FWidgetsLayoutBP* WidgetLayout = Cast<AMyHUD>(PC->HUD_Reference)->GetWidgetBPClass("ItemTooltip_WBP");

	UW_ItemTooltip* Tooltip{};
	FSlotStructure TempSlot{};
	USlotLayout* SlotLayout{};

	uint8 Index = 0;
	uint8 LocalIndex = 0;

	for (FSlotStructure Slot : InOtherInventory)
	{
		TempSlot = GetEmptySlot(EEquipmentSlot::Undefined);

		// Get SlotLayout Reference
		SlotLayout = MainLayoutUI->Container->ContainerSlotsArray[Index];

		// Validate Tooltip
		Tooltip = Cast<UW_ItemTooltip>(SlotLayout->ToolTipWidget);
		if (!IsValid(Tooltip))
		{
			Tooltip = CreateWidget<UW_ItemTooltip>(GetWorld(), WidgetLayout->Widget);

			UE_LOG(LogTemp, Verbose, TEXT("Creating Container Tooltip"))
		}

		if (Slot.ItemStructure.ItemType == EItemType::Equipment)
		{
			for (uint8 j = 0; j < (uint8)EEquipmentSlot::Count; j++)
			{
				if (GetItemEquipmentSlot(InPlayerInventory[j]) == GetItemEquipmentSlot(Slot))
				{
					TempSlot = InPlayerInventory[j];
					break;
				}
			}
		}

		Tooltip->UpdateTooltipAttributes(Slot.ItemStructure, TempSlot);
		SlotLayout->ToggleTooltip();
		SlotLayout->SetToolTip(Tooltip);

		Index++;
	}
}

void UInventoryManagerComponent::Client_UpdateInventoryTooltips_Implementation(const TArray<FSlotStructure>& InPlayerInventory, const TArray<FSlotStructure>& InOtherInventory)
{
	AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwner());
	if (!IsValid(PC))
	{
		return;
	}
	FWidgetsLayoutBP* WidgetLayout = Cast<AMyHUD>(PC->HUD_Reference)->GetWidgetBPClass("ItemTooltip_WBP");

	UW_ItemTooltip* Tooltip{};
	FSlotStructure TempSlot{};
	USlotLayout* SlotLayout{};

	uint8 Index = 0;
	uint8 LocalIndex = 0;

	for (FSlotStructure Slot : InOtherInventory)
	{
		TempSlot = GetEmptySlot(EEquipmentSlot::Undefined);

		// Get SlotLayout Reference
		if (Index < (uint8)EEquipmentSlot::Count)
		{
			SlotLayout = MainLayoutUI->Profile->EquipmentSlotsArray[Index];
		}
		else
		{
			LocalIndex = Index - (uint8)EEquipmentSlot::Count;
			SlotLayout = MainLayoutUI->Inventory->InventorySlotsArray[LocalIndex];
		}

		// Validate Tooltip
		Tooltip = Cast<UW_ItemTooltip>(SlotLayout->ToolTipWidget);
		if (!IsValid(Tooltip))
		{
			// Tooltips must be initialized on Load
			Tooltip = CreateWidget<UW_ItemTooltip>(GetWorld(), WidgetLayout->Widget);

			UE_LOG(LogTemp, Verbose, TEXT("Creating Inventory Tooltip"))
		}

		if (Slot.ItemStructure.ItemType == EItemType::Equipment)
		{
			for (uint8 j = 0; j < (uint8)EEquipmentSlot::Count; j++)
			{
				if (GetItemEquipmentSlot(InPlayerInventory[j]) == GetItemEquipmentSlot(Slot))
				{
					TempSlot = InPlayerInventory[j];
				}
			}
		}

		Tooltip->UpdateTooltipAttributes(Slot.ItemStructure, TempSlot);
		SlotLayout->ToggleTooltip();
		SlotLayout->SetToolTip(Tooltip);

		Index++;
	}
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
	else if (LocalInventory == ContainerInventory)
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
			}
			else
			{
				AddItem(ToInventory, ToInventorySlot, LocalInventoryItem);
				RemoveItem(FromInventory, FromInventorySlot);
			}

			UpdateEquippedStats();

			Server_UpdateTooltips();

			return;

		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("ITEM CAN NOT BE EQUIPPED IN THAT SLOT"))
		}
	}
	else
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

	Server_UpdateTooltips();
}

void UInventoryManagerComponent::RandomizeDropLocation(FSlotStructure LocalSlot, UClass*& LocalClass, FTransform& OutTransform)
{
	LocalClass = LocalSlot.ItemStructure.Class;

	// Drop at character feet
	FVector LocalLocation{ 0.0f, 0.0f, -98.0f };

	FVector PawnLocation = Cast<AController>(GetOwner())->GetPawn()->GetActorLocation();

	//Drop Distance Range From Character
	//const uint8 DropDistanceRangeX = FMath::RandRange(64, 96);
	const uint8 DropDistanceRangeX = FMath::RandRange(75, 100);
	FVector DistanceFromPawn{ (float)DropDistanceRangeX,1.0f,1.0f };

	// Drop Items 360 Degrees Around Player
	const float DropItemsRotation = FMath::FRandRange(-180, 180);
	FRotator Rotation{ 1.0f, DropItemsRotation, DropItemsRotation }; // Drop Around Player
	//FRotator Rotation {1.0f, 1.0f, DropItemsRotation}; // Drop In One Point

	FVector VectorRotated = Rotation.RotateVector(DistanceFromPawn);

	FVector FinalLocation = PawnLocation + LocalLocation + VectorRotated;

	// Give The Dropped Object A Random Rotation
	// const int8 RandomRotation = FMath::RandRange(-10, 10);
	// FRotator FinalRotator {0.0f, 0.0f, (float)RandomRotation * 10};
	FRotator FinalRotator{ 1.0f, 1.0f, 1.0f };

	FVector FinalScale{ 1.0f,1.0f,1.0f };

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
	}
	else
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
	if (ItemIsValid(LocalSwapInventoryItem))
	{
		// Check if Stackable
		if (ItemIsSame(LocalInventoryItem, LocalSwapInventoryItem) &&
			IsItemStackable(LocalSwapInventoryItem) &&
			ItemFreeStackSpace(LocalSwapInventoryItem) > 0)
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

				Server_UpdateTooltips();
				return;
			}
			else
			{
				// Full Amount Was Moved Clear Old Stack
				RemoveItem(FromInventory, FromInventorySlot);

				Server_UpdateTooltips();
				return;
			}
		}
		else {
			// Swap Items
			AddItem(ToInventory, ToInventorySlot, LocalInventoryItem);
			AddItem(FromInventory, FromInventorySlot, LocalSwapInventoryItem);
		}
	}
	else
	{
		// Move Item
		AddItem(ToInventory, ToInventorySlot, LocalInventoryItem);
		RemoveItem(FromInventory, FromInventorySlot);
	}

	Server_UpdateTooltips();
}

void UInventoryManagerComponent::AddItemToStack(UInventoryComponent* Inventory, uint8 InventorySlot, uint8 AmountToAdd,
	uint8& AmountRemaining)
{
	uint8 LocalRemainingAmount = AmountToAdd;

	FSlotStructure LocalInventoryItem = Inventory->GetInventoryItem(InventorySlot);

	// Does Stack Has Free Space?
	if (GetItemAmount(LocalInventoryItem) < GetItemMaxStackSize(LocalInventoryItem))
	{
		// Does The Full Amount To Add Fit In Stack?
		if (AmountToAdd <= ItemFreeStackSpace(LocalInventoryItem))
		{	
			LocalRemainingAmount = 0;
			
			AddAmountToItem(LocalInventoryItem, AmountToAdd);
			AddItem(Inventory, InventorySlot, LocalInventoryItem);
		}
		else
		{
			LocalRemainingAmount = AmountToAdd - ItemFreeStackSpace(LocalInventoryItem);
			LocalInventoryItem.Amount = GetItemMaxStackSize(LocalInventoryItem);
			AddItem(Inventory, InventorySlot, LocalInventoryItem);
		}
	}

	AmountRemaining = LocalRemainingAmount;

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
	}
	else
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
	}
	else
	{
		ClearViewersContainerSlot(InventorySlot);
	}
}

void UInventoryManagerComponent::UseContainer(AActor* Container)
{
	if (Container->Implements<UInventoryInterface>())
	{
		if (CurrentContainer != Container)
		{
			OpenContainer(Container);
		}
		else
		{
			Server_CloseContainer();
		}
	}
}

FSlotStructure UInventoryManagerComponent::GetEmptySlot(const EEquipmentSlot FromEquipmentType)
{
	FName Name;
	if (FromEquipmentType == EEquipmentSlot::Weapon)
	{
		Name = "No_Weapon";
	}
	else if (FromEquipmentType == EEquipmentSlot::Chest)
	{
		Name = "No_Chest";
	}
	else if (FromEquipmentType == EEquipmentSlot::Feet)
	{
		Name = "No_Feet";
	}
	else if (FromEquipmentType == EEquipmentSlot::Hands)
	{
		Name = "No_Hands";
	}
	else
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

bool UInventoryManagerComponent::ItemIsSame(const FSlotStructure Item1, const FSlotStructure Item2)
{
	if (Item1.ItemStructure.ID == Item2.ItemStructure.ID)
	{
		return true;
	}
	return false;
}

bool UInventoryManagerComponent::IsItemStackable(const FSlotStructure Item) {
	return Item.ItemStructure.IsStackable;
}

uint8 UInventoryManagerComponent::ItemFreeStackSpace(const FSlotStructure Item) {
	return GetItemMaxStackSize(Item) - GetItemAmount(Item);
}

uint8 UInventoryManagerComponent::GetItemAmount(const FSlotStructure Item) {
	return Item.Amount;
}

uint8 UInventoryManagerComponent::GetItemMaxStackSize(const FSlotStructure Item) {
	return Item.ItemStructure.MaxStackSize;
}

void UInventoryManagerComponent::AddAmountToItem(FSlotStructure& Item, uint8 AmountToAdd) {
	Item.Amount += AmountToAdd;
}

void UInventoryManagerComponent::RemoveFromItemAmount(FSlotStructure& InventoryItem, const uint8& AmountToRemove,
	bool& WasFullAmountRemoved, uint8& AmountRemoved)
{
	if (AmountToRemove >= InventoryItem.Amount)
	{
		AmountRemoved = InventoryItem.Amount;

		WasFullAmountRemoved = true;
	}
	else
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
		USlotLayout* SlotLayout{};
		if (InventorySlot >= (uint8)EEquipmentSlot::Count)
		{
			uint8 LocalIndex = InventorySlot - (uint8)EEquipmentSlot::Count;
			SlotLayout = MainLayoutUI->Inventory->InventorySlotsArray[LocalIndex];
		}
		else
		{
			SlotLayout = MainLayoutUI->Profile->EquipmentSlotsArray[InventorySlot];
		}

		SlotLayout->UpdateSlot(ContentToAdd);
	}
}

void UInventoryManagerComponent::ClearInventorySlotItem(uint8 InventorySlot)
{
	if (IsValid(MainLayoutUI))
	{
		USlotLayout* SlotLayout{};
		FSlotStructure LocalSlot{};
		if (InventorySlot >= (uint8)EEquipmentSlot::Count)
		{
			uint8 LocalIndex = InventorySlot - (uint8)EEquipmentSlot::Count;

			SlotLayout = MainLayoutUI->Inventory->InventorySlotsArray[LocalIndex];
			LocalSlot = GetEmptySlot(EEquipmentSlot::Undefined);
		}
		else
		{
			SlotLayout = MainLayoutUI->Profile->EquipmentSlotsArray[InventorySlot];
		
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
		}

		SlotLayout->UpdateSlot(LocalSlot);
	}
}

FSlotStructure UInventoryManagerComponent::GetInventorySlotItem(uint8 InventorySlot)
{
	uint8 Index = InventorySlot;
	if (InventorySlot >= (uint8)EEquipmentSlot::Count)
	{
		Index = InventorySlot - (uint8)EEquipmentSlot::Count;
	}

	FSlotStructure Slot = MainLayoutUI->Inventory->InventorySlotsArray[Index]->SlotStructure;
	return Slot;
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
	for (uint8 Index = 0; Index < (uint8)EEquipmentSlot::Count; Index++)
	{
		EEquipmentSlot LocalSlotType = GetEquipmentTypeBySlot(Index);
		if (EquipmentSlot == LocalSlotType)
		{
			return Index;
		}
	}
	return 0;
}

void UInventoryManagerComponent::Server_UseInventoryItem_Implementation(const uint8& InventorySlot)
{
	UseInventoryItem(InventorySlot);
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
			}
			else
			{
				AddItem(PlayerInventory, InventorySlot, LocalInventorySlot);
			}
			break;
		}
	}
	else
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

	Server_UpdateTooltips();
}

void UInventoryManagerComponent::UseConsumableItem(uint8 InventorySlot, FSlotStructure InventoryItem)
{
	// Do something depending on the item properties if needed
	// ...
	//if (Cast<AMyPlayerController>(GetOwner()))
	//{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Consuming this Item...")));
		UE_LOG(LogTemp, Warning, TEXT("Consuming this Item..."))
	//}

	// Remove from Item Amount
	uint8 AmountToRemove = 1;
	bool WasFullAmountRemoved = false;
	uint8 AmountRemoved = 0;

	RemoveFromItemAmount(InventoryItem, AmountToRemove, WasFullAmountRemoved, AmountRemoved);

	if (WasFullAmountRemoved)
	{
		InventoryItem = GetEmptySlot(EEquipmentSlot::Undefined);

		RemoveItem(PlayerInventory, InventorySlot);
	}
	else
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
		}
		else if (ToInventory == PlayerInventory)
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
		if (GetEmptyEquipmentSlotByType(LocalEquipmentSlot, OutInventorySlot))
		{
			Server_EquipFromInventory(InventorySlot, OutInventorySlot);
		}
		else
		{
			OutInventorySlot = GetEquipmentSlotByType(LocalEquipmentSlot);
			Server_EquipFromInventory(InventorySlot, OutInventorySlot);
		}
	}
}

void UInventoryManagerComponent::Server_EquipFromContainer_Implementation(uint8 FromInventorySlot, uint8 ToInventorySlot)
{
	EquipItem(ContainerInventory, FromInventorySlot, PlayerInventory, ToInventorySlot);

	Server_UpdateTooltips();
}

void UInventoryManagerComponent::Server_UnEquipToContainer_Implementation(uint8 FromInventorySlot, uint8 ToInventorySlot)
{
	UnEquipItem(PlayerInventory, FromInventorySlot, ContainerInventory, ToInventorySlot);

	Server_UpdateTooltips();
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

	/* Client Container Events */

void UInventoryManagerComponent::Server_MoveContainerItem_Implementation(uint8 FromInventorySlot, uint8 ToInventorySlot)
{
	MoveItem(ContainerInventory, FromInventorySlot, ContainerInventory, ToInventorySlot);
}

void UInventoryManagerComponent::Server_UseContainer_Implementation(AActor* Container)
{
	UseContainer(Container);
}

void UInventoryManagerComponent::Server_CloseContainer_Implementation()
{
	CloseContainer();
}

void UInventoryManagerComponent::Server_UseContainerItem_Implementation(const uint8& InventorySlot)
{
	UseContainerItem(InventorySlot);
}

void UInventoryManagerComponent::UseContainerItem(const uint8& InventorySlot)
{
	FSlotStructure LocalInventoryItem = ContainerInventory->GetInventoryItem(InventorySlot);

	bool bOutSuccess = false;
	TryToAddItemToInventory(PlayerInventory, LocalInventoryItem, bOutSuccess);

	if (bOutSuccess)
	{
		RemoveItem(ContainerInventory, InventorySlot);
	}
	else
	{
		AddItem(ContainerInventory, InventorySlot, LocalInventoryItem);
	}

	Server_UpdateTooltips();
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

	/* Client Container UI Events */

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

void UInventoryManagerComponent::Client_ClearContainerSlotItem_Implementation(uint8 ContainerSlot)
{
	ClearContainerSlotItem(ContainerSlot);
}

void UInventoryManagerComponent::ClearContainerSlotItem(uint8 ContainerSlot)
{
	USlotLayout* SlotLayout = MainLayoutUI->Container->ContainerSlotsArray[ContainerSlot];
	SlotLayout->UpdateSlot(GetEmptySlot(EEquipmentSlot::Undefined));
}

void UInventoryManagerComponent::SetContainerSlotItem(const FSlotStructure& Slot, uint8 Index)
{
	USlotLayout* SlotLayout = MainLayoutUI->Container->ContainerSlotsArray[Index];
	SlotLayout->UpdateSlot(Slot);
}

void UInventoryManagerComponent::Client_CloseContainer_Implementation()
{
	MainLayoutUI->Container->SetVisibility(ESlateVisibility::Hidden);
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

void UInventoryManagerComponent::Client_SetContainerSlotItem_Implementation(const FSlotStructure& ContentToAdd,
	const uint8& InventorySlot)
{
	SetContainerSlotItem(ContentToAdd, InventorySlot);
}

void UInventoryManagerComponent::Client_ClearInventorySlotItem_Implementation(uint8 InventorySlot)
{
	ClearInventorySlotItem(InventorySlot);
}

void UInventoryManagerComponent::Client_OpenContainer_Implementation(FContainerInfo ContainerProperties,
	const TArray<FSlotStructure>& InContainerInventory, const TArray<FSlotStructure>& InPlayerInventory)
{
	LoadContainerSlots(ContainerProperties, InContainerInventory, InPlayerInventory);
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

	Client_OpenContainer(C_Info, LocalInventory, PlayerInventory->Inventory);
}

void UInventoryManagerComponent::LoadContainerSlots(FContainerInfo ContainerProperties,
	const TArray<FSlotStructure>& InContainerInventory, const TArray<FSlotStructure>& InPlayerInventory)
{
	CreateContainerSlots(ContainerProperties.NumberOfRows, ContainerProperties.SlotsPerRow);

	uint8 Index = 0;
	for (FSlotStructure Slot : InContainerInventory)
	{
		SetContainerSlotItem(Slot, Index);

		Index++;
	}

	Client_UpdateContainerTooltips(InPlayerInventory, InContainerInventory);

	if (AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwner()))
	{
		PC->ToggleContainer();
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

		for (int i = 0; i < NumberOfRows; i++)
		{
			for (int j = 0; j < SlotsPerRow; j++)
			{
				W_ContainerSlot = CreateWidget<USlotLayout>(GetWorld(), WidgetLayout->Widget);
				MainLayoutUI->Container->ContainerGridPanel->AddChildToUniformGrid(W_ContainerSlot, i, j);

				MainLayoutUI->Container->ContainerSlotsArray.Add(W_ContainerSlot);
			}
		}

		FSlotStructure SlotStructure = GetEmptySlot(EEquipmentSlot::Undefined);

		for (int i = 0; i < MainLayoutUI->Container->ContainerSlotsArray.Num(); i++)
		{
			MainLayoutUI->Container->ContainerSlotsArray[i]->UpdateSlot(SlotStructure);
			MainLayoutUI->Container->ContainerSlotsArray[i]->InventorySlotIndex = i;
			MainLayoutUI->Container->ContainerSlotsArray[i]->NativeFromContainer = true;

			MainLayoutUI->Container->ContainerSlotsArray[i]->IsStorageSlot = true;
		}
	}
}

	/* Client Only - Hotbar Events */

void UInventoryManagerComponent::Client_MoveHotbarSlotItem_Implementation(const uint8& FromSlot, const uint8& ToSlot, const bool IsDraggedFromInventory, const bool IsDraggedFromHotbar)
{
	MoveHotbarSlotItem(FromSlot, ToSlot, IsDraggedFromInventory, IsDraggedFromHotbar);
}

void UInventoryManagerComponent::Client_UseHotbarSlot_Implementation(const uint8& HotbarSlot)
{
	UseHotbarSlot(HotbarSlot);
}

void UInventoryManagerComponent::Client_ClearHotbarSlot_Implementation(const uint8& HotbarSlot)
{
	ClearHotbarSlotItem(HotbarSlot);
}

	/* Hotbar Events */

void UInventoryManagerComponent::MoveHotbarSlotItem(const uint8& FromSlot, const uint8& ToSlot, const bool IsDraggedFromInventory, const bool IsDraggedFromHotbar)
{
	if (IsDraggedFromInventory)
	{
		FSlotStructure SlotStructure = GetInventorySlotItem(FromSlot);

		SetHotbarSlotItem(ToSlot, SlotStructure);
	}

	if (IsDraggedFromHotbar)
	{
		FSlotStructure FromSlotItem = GetHotbarSlotItem(FromSlot);
		FSlotStructure ToSlotItem = GetHotbarSlotItem(ToSlot);

		if (ItemIsValid(ToSlotItem))
		{
			SetHotbarSlotItem(ToSlot, FromSlotItem);
			SetHotbarSlotItem(FromSlot, ToSlotItem);
		}
		else {
			SetHotbarSlotItem(ToSlot, FromSlotItem);
			ClearHotbarSlotItem(FromSlot);
		}
	}
}

void UInventoryManagerComponent::UseHotbarSlot(const uint8& HotbarSlot)
{
	FReturnTupleBoolInt Tuple{};
	FSlotStructure Slot = GetHotbarSlotItem(HotbarSlot);

	if (ItemIsValid(Slot))
	{
		TArray<USlotLayout*> LocalInventoryUI = MainLayoutUI->Inventory->InventorySlotsArray;
		for(uint8 i = 0; i < LocalInventoryUI.Num(); i++) {
			if (Slot.ItemStructure.ID == LocalInventoryUI[i]->SlotStructure.ItemStructure.ID) {
				Tuple.Success = true;
				Tuple.Index = i;
				break;
			}
		}
		
		if (Tuple.Success)
		{
			Tuple.Index = Tuple.Index + (uint8)EEquipmentSlot::Count;
			// Get And Use The Real Item On The Server
			Server_UseInventoryItem(Tuple.Index);
		}
	}
}

void UInventoryManagerComponent::ClearHotbarSlotItem(const uint8& HotbarSlot)
{
	UHotbar_Slot* Slot = MainLayoutUI->Hotbar->HotbarSlotsArray[HotbarSlot];

	FSlotStructure LocalSlot = GetEmptySlot(EEquipmentSlot::Undefined);

	Slot->UpdateSlot(LocalSlot);
	
}

void UInventoryManagerComponent::SetHotbarSlotItem(const uint8& ToSlot, FSlotStructure SlotStructure)
{
	UHotbar_Slot* HotbarSlot = MainLayoutUI->Hotbar->HotbarSlotsArray[ToSlot];

	HotbarSlot->UpdateSlot(SlotStructure);
}

FSlotStructure UInventoryManagerComponent::GetHotbarSlotItem(const uint8& HotbarSlot)
{
	TArray<UHotbar_Slot*> HotbarSlotsArray =  MainLayoutUI->Hotbar->HotbarSlotsArray;
	FSlotStructure Slot{};

	for (uint8 i = 0; i < HotbarSlotsArray.Num(); i++)
	{
		if (HotbarSlotsArray[i]->HotbarSlotIndex == HotbarSlot) {
			Slot = HotbarSlotsArray[i]->SlotStructure;
		}
	}

	return Slot;
}
