// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/EEquipmentSlot.h"
#include "Inventory/EItemType.h"
#include "Inventory/FContainerInfo.h"
#include "Inventory/FSlotStructure.h"
#include "InventoryManagerComponent.generated.h"

class UDataTable;
class UMainLayout;
class UInventoryComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYSYSTEMCPP_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryManagerComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void InitializeInventoryManager(UInventoryComponent* EquipmentComponent);
	
	UFUNCTION(Server, Reliable)
	virtual void Server_InitInventory();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Player Inventory", Category="Default", OverrideNativeName="PlayerInventory"))
	UInventoryComponent* PlayerInventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Container Inventory", Category="Default", OverrideNativeName="ContainerInventory"))
	UInventoryComponent* ContainerInventory;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	uint8 TotalNumberOfSlots;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UserInterface")
	UMainLayout* MainLayoutUI;

	UFUNCTION(Category="Manager|Public")
	void TryToAddItemToInventory(UInventoryComponent* Inventory, FSlotStructure& InventoryItem, bool& bOutSuccess);
	
	UFUNCTION(Server, Reliable)
	void Server_UseInventoryItem(const uint8& InventorySlot);

	UFUNCTION(Server, Reliable)
	void Server_UseContainerItem(const uint8& InventorySlot);

	UFUNCTION(Server, Reliable)
	void Server_MoveInventoryItem(uint8 FromInventorySlot, uint8 ToInventorySlot);
	
	UFUNCTION(Server, Reliable)
	void Server_EquipFromInventory(uint8 FromInventorySlot, uint8 ToInventorySlot);

	UFUNCTION(Server, Reliable)
	void Server_UnEquipToInventory(uint8 FromInventorySlot, uint8 ToInventorySlot);
	
	UFUNCTION(Server, Reliable)
	void Server_DropItemFromInventory(const uint8& InventorySlot);

	UFUNCTION(Server, Reliable)
	void Server_UseContainer(AActor* Container);
	UFUNCTION(Server, Reliable)
	void Server_CloseContainer();

	UFUNCTION(Server, Reliable)
	void Server_TakeContainerItem(const uint8& FromContainerSlot, const uint8& ToInventorySlot);
	UFUNCTION(Server, Reliable)
	void Server_DepositContainerItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot);
	UFUNCTION(Server, Reliable)
	void Server_MoveContainerItem(uint8 FromInventorySlot, uint8 ToInventorySlot);
	
	UFUNCTION(Server, Reliable)
	void Server_EquipFromContainer(uint8 FromInventorySlot, uint8 ToInventorySlot);
	UFUNCTION(Server, Reliable)
	void Server_UnEquipToContainer(uint8 FromInventorySlot, uint8 ToInventorySlot);
	
	UFUNCTION(Client, Reliable)
	void Client_SetInventorySlotItem(const FSlotStructure& ContentToAdd, const uint8& InventorySlot);
	UFUNCTION(Client, Reliable)
	void Client_SetContainerSlotItem(const FSlotStructure& ContentToAdd, const uint8& InventorySlot);

	UFUNCTION(Client, Reliable)
	void Client_ClearInventorySlotItem(uint8 InventorySlot);
	UFUNCTION(Client, Reliable)
	void Client_ClearContainerSlotItem(uint8 ContainerSlot);

	UFUNCTION(Client, Reliable)
	void Client_OpenContainer(FContainerInfo ContainerProperties, const TArray<FSlotStructure>& InContainerInventory, const TArray<FSlotStructure>& InPlayerInventory);
	UFUNCTION(Client, Reliable)
	void Client_CloseContainer();
	
	UFUNCTION(Client, Reliable)
		void Client_MoveHotbarSlotItem(const uint8& FromSlot, const uint8& ToSlot, const bool IsDraggedFromInventory, const bool IsDraggedFromHotbar);
	UFUNCTION(Client, Reliable)
		void Client_UseHotbarSlot(const uint8& HotbarSlot);
	UFUNCTION(Client, Reliable)
		void Client_ClearHotbarSlot(const uint8& HotbarSlot);

	UFUNCTION(Server, Reliable)
		void Server_UpdateTooltips();

	UFUNCTION(Client, Reliable)
		void Client_LoadInventoryUI();
	UFUNCTION(Client, Reliable)
		void Client_LoadProfileUI();
	UFUNCTION(Client, Reliable)
		void Client_LoadHotbarUI();

	UFUNCTION(Client, Reliable)
		void Client_CheckHotbarSlots(const FSlotStructure& Slot);

	UFUNCTION()
		void LoadHotbarUI();

	UFUNCTION(Category="Manager|Public")
	void InitializeInventoryManagerUI(UMainLayout* MainLayout);
	
	UFUNCTION(Category="Manager|Private")
	EEquipmentSlot GetEquipmentTypeBySlot(const uint8& EquipmentSlot);
	
	UFUNCTION()
	EItemType GetItemTypeBySlot(uint8 ItemSlot);

	UFUNCTION(Category = "Manager|Private|Inventory")
	void AddItem(UInventoryComponent* Inventory, uint8 InventorySlot, FSlotStructure& InventoryItem);

	UFUNCTION()
	FSlotStructure GetEmptySlot(EEquipmentSlot FromEquipmentType);
	
	FSlotStructure GetItemFromItemDB(FName Name);
	
	UFUNCTION(Client, Reliable)
	void Client_SetAttributes(const TArray<uint8>& InAttributesArray);
	
	UFUNCTION()
	void UpdateEquippedStats();

	UFUNCTION(Category="Manager|Getter")
	UDataTable* GetItemDB();

	UFUNCTION()
	void InitializePlayerAttributes();
	
	UPROPERTY(Replicated, EditAnywhere,BlueprintReadWrite)
	TArray<uint8> AttributesArray;

	UPROPERTY()
	AActor* CurrentContainer;
	
	/*Defined on Player Controller*/
	uint8 NumberOfRowsInventory = 0;
	uint8 SlotsPerRowInventory = 0;
	uint8 NumberOfSlotsOnHotbar = 0;

protected:
	UFUNCTION(Client, Reliable)
		void Client_UpdateInventoryTooltips(const TArray<FSlotStructure>& InPlayerInventory, const TArray<FSlotStructure>& InOtherInventory);
	UFUNCTION(Client, Reliable)
		void Client_UpdateContainerTooltips(const TArray<FSlotStructure>& InPlayerInventory, const TArray<FSlotStructure>& InOtherInventory);

private:
	UFUNCTION(Category = "UserInterface|Private|Hotbar")
		void MoveHotbarSlotItem(const uint8& FromSlot, const uint8& ToSlot, const bool IsDraggedFromInventory, const bool IsDraggedFromHotbar);
	UFUNCTION(Category = "UserInterface|Private|Hotbar")
		void UseHotbarSlot(const uint8& HotbarSlot);
	UFUNCTION(Category = "UserInterface|Private|Hotbar")
		void ClearHotbarSlotItem(const uint8& HotbarSlot);

	UFUNCTION(Category = "UserInterface|Private|Hotbar")
		void SetHotbarSlotItem(const uint8& ToSlot, FSlotStructure SlotStructure);
	UFUNCTION(Category = "UserInterface|Private|Hotbar")
		FSlotStructure GetHotbarSlotItem(const uint8& HotbarSlot);

	UPROPERTY()
	UDataTable* ItemDB;
	
	UFUNCTION(Category = "Helper")
	uint8 GetEquipmentSlotByType(EEquipmentSlot EquipmentSlot);
	UFUNCTION(Category = "Helper")
	EEquipmentSlot GetItemEquipmentSlot(FSlotStructure Item);
	UFUNCTION(Category = "Helper")
	bool ItemIsValid(FSlotStructure Item);
	UFUNCTION(Category = "Helper")
	bool GetEmptyEquipmentSlotByType(EEquipmentSlot EquipmentSlot, uint8& OutIndex);
	UFUNCTION(Category = "Helper")
	bool ItemIsSame(const FSlotStructure Item1, const FSlotStructure Item2);
	UFUNCTION(Category = "Helper")
	bool IsItemStackable(const FSlotStructure Item);
	UFUNCTION(Category = "Helper")
	uint8 ItemFreeStackSpace(const FSlotStructure Item);
	UFUNCTION(Category = "Helper")
	uint8 GetItemAmount(const FSlotStructure Item);
	UFUNCTION(Category = "Helper")
	uint8 GetItemMaxStackSize(const FSlotStructure Item);
	UFUNCTION(Category = "Helper")
	void AddAmountToItem(FSlotStructure & Item, uint8 AmountToAdd);

	UFUNCTION()
		void SetAttributes(const TArray<uint8>& InAttributesArray);

	// Remove a specific amount from an existing item on Inventory
	UFUNCTION()
	void RemoveFromItemAmount(FSlotStructure& InventoryItem, const uint8& AmountToRemove, bool& WasFullAmountRemoved, uint8& AmountRemoved);

	UFUNCTION(Category="Manager|Private|Equipment")
	void EquipItem(UInventoryComponent* FromInventory, uint8 FromInventorySlot,
										  UInventoryComponent* ToInventory, uint8 ToInventorySlot);
	
	UFUNCTION(Category="Manager|Private|Equipment")
	void UnEquipItem(UInventoryComponent* FromInventory, uint8 FromInventorySlot,
										  UInventoryComponent* ToInventory, uint8 ToInventorySlot);
	
	UFUNCTION(Category="Manager|Private")
	void RandomizeDropLocation(FSlotStructure LocalSlot, UClass*& LocalClass, FTransform& OutTransform);

	UFUNCTION(Category="Manager|Private|Inventory")
	void DropItem(UInventoryComponent* Inventory, uint8 InventorySlot);

	UFUNCTION(Category="Manager|Private|Inventory")
	void MoveItem(UInventoryComponent* FromInventory, uint8 FromInventorySlot, UInventoryComponent* ToInventory, uint8 ToInventorySlot);
	UFUNCTION(Category="Manager|Private|Inventory")
	void RemoveItem(UInventoryComponent* Inventory, uint8 InventorySlot);

	// Equips Or Unequips The Inventory Item When Used Or Moves To A Container (If A Storage Container Is Open)
	UFUNCTION(Category="Manager|Private|Items")
	void UseEquipmentItem(uint8 InventorySlot, FSlotStructure InventoryItem, UInventoryComponent* ToInventory);

	//Consumes The Item When Used
	UFUNCTION(Category="Manager|Private|Items")
	void UseConsumableItem(uint8 InventorySlot, FSlotStructure InventoryItem);
	
	//Server: Use Item In Inventory. Equips, Consumes Or Moves To A Container (If A Storage Container Is Open)
	UFUNCTION(Category="Manager|Private|Items")
	void UseInventoryItem(const uint8& InventorySlot);
	//Server: Use Item From Container (Moves It To Inventory)
	UFUNCTION(Category="Manager|Private|Container")
	void UseContainerItem(const uint8& InventorySlot);

	UFUNCTION(Category="UserInterface|Private|Inventory")
	void ClearInventoryItem(const uint8& InventorySlot);
	UFUNCTION(Category="UserInterface|Private|Inventory")
	void SetInventorySlotItem(const FSlotStructure& ContentToAdd, const uint8& InventorySlot);
	UFUNCTION(Category="UserInterface|Private|Inventory")
	void ClearInventorySlotItem(uint8 InventorySlot);
	
	UFUNCTION(Category = "UserInterface|Private|Inventory")
	FSlotStructure GetInventorySlotItem(uint8 InventorySlot);


	UFUNCTION(Category="UserInterface|Private|Container") 
	void ClearContainerSlots();
	UFUNCTION(Category="UserInterface|Private|Container")
	void CreateContainerSlots(uint8 NumberOfRows, uint8 SlotsPerRow);
	UFUNCTION(Category="UserInterface|Private|Container")
	void SetViewersContainerSlot(uint8 ContainerSlot, FSlotStructure& InventoryItem);

	UFUNCTION(Category="UserInterface|Private|Container")
	void ClearContainerSlotItem(uint8 ContainerSlot);
	UFUNCTION(Category="UserInterface|Private|Container")
	void ClearViewersContainerSlot(uint8 ContainerSlot);
	UFUNCTION(Category="UserInterface|Private|Container")
	void SetContainerSlotItem(const FSlotStructure& Slot, uint8 Index);

	UFUNCTION(Category="Manager|Private|Container")
	void OpenContainer(AActor* Container);
	UFUNCTION(Category="Manager|Private|Container")
	void UseContainer(AActor* Container);
	UFUNCTION(Category="Manager|Private|Container")
	void CloseContainer();
	UFUNCTION(Category="Manager|Private|Container")
	void LoadContainerSlots(FContainerInfo ContainerProperties, const TArray<FSlotStructure>& InContainerInventory, const TArray<FSlotStructure>& InPlayerInventory);
	
	UFUNCTION(Category = "Manager|Private")
	bool CanContainerStoreItems(UInventoryComponent* Inventory);

	UFUNCTION(Category="Manager|Private|Stacks")
	void FindAndAddAmountToStacks(UInventoryComponent* Inventory, FName ItemID, uint8 Amount, uint8& AmountRemaining);
	
	UFUNCTION(Category="Manager|Private|Stacks")
	void AddItemToStack(UInventoryComponent* Inventory, uint8 InventorySlot, uint8 AmountToAdd, uint8& AmountRemaining);
};