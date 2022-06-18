// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/FContainerInfo.h"
#include "ContainerActor.h"
#include "EquipmentComponent.h"
#include "InventoryComponent.h"
#include "Tuples.h"
#include "Components/ActorComponent.h"
#include "Inventory/FSlotStructure.h"
#include "MyCharacter.h"
#include "UI/MainLayout.h"
#include "InventoryManagerComponent.generated.h"

struct FContainerInfo;

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
	
	virtual bool InitInventory(uint8 NumberSlots);

	UFUNCTION(Server, Reliable)
	void Server_InitInventory();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Player Inventory", Category="Default", OverrideNativeName="PlayerInventory"))
	UInventoryComponent* PlayerInventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Container Inventory", Category="Default", OverrideNativeName="ContainerInventory"))
	UInventoryComponent* ContainerInventory;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	uint8 NumberOfSlots;

	UPROPERTY()
	AMyPlayerController* ControllerReference;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UserInterface")
	UMainLayout* InventoryUI;
	
	UPROPERTY()
	AMyCharacter* CharacterReference;
	
	UFUNCTION(Server, Reliable)
	void Server_EquipFromInventory(const uint8& FromInventorySlot, const uint8& ToInventorySlot);

	UFUNCTION(Server, Reliable)
	void Server_UnEquipFromInventory(const uint8& FromInventorySlot, const uint8& ToInventorySlot);
	
	UFUNCTION(Server, Reliable)
	void Server_DropItemFromInventory(const uint8& InventorySlot);

	UFUNCTION(Client, Reliable)
	void Client_SetInventorySlotItem(const FSlotStructure& ContentToAdd, const uint8& InventorySlot);

	UFUNCTION(Server, Reliable)
	void Server_UseContainer(AActor* Container);
	UFUNCTION(Server, Reliable)
	void Server_CloseContainer();

	UFUNCTION(Client, Reliable)
	void Client_OpenContainer(FContainerInfo ContainerProperties, const TArray<FSlotStructure>& InContainerInventory);
	UFUNCTION(Client, Reliable)
	void Client_CloseContainer();

	UFUNCTION(Server, Reliable)
	void Server_TakeContainerItem(const uint8& FromContainerSlot, const uint8& ToInventorySlot);
	UFUNCTION(Server, Reliable)
	void Server_DepositContainerItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot);
	
	UFUNCTION(Client, Reliable)
	void Client_SetContainerSlot(const FSlotStructure& ContentToAdd, const uint8& InventorySlot);

	UFUNCTION(Category= "Manager|Public")
	void InitializeInventoryManagerUI(UMainLayout* MainLayout);
	
	UFUNCTION()
	EEquipmentSlot GetEquipmentTypeBySlot(const uint8& EquipmentSlot);
	UFUNCTION()
	EItemType GetItemTypeBySlot(const uint8& ItemSlot);
	
	UFUNCTION()
	bool AddItem(FName ID, uint8 Amount);

	UFUNCTION()
	bool AddItemToInventory(FSlotStructure& ContentToAdd);

	UFUNCTION()
	FSlotStructure GetInventorySlot(const uint8& InventorySlot);

	UFUNCTION()
	FSlotStructure GetEmptySlot(EEquipmentSlot FromEquipmentType);
	
	FSlotStructure GetItemFromItemDB(FName Name);

	UFUNCTION()
	void UseInventoryItem(const uint8& InventorySlot);
	
	UFUNCTION()
	virtual bool MoveInventoryItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot);

	UFUNCTION()
	void UpdateEquippedMeshes(const uint8& InventorySlot);
	UFUNCTION()
	void UpdateEquippedStats();
	
	UFUNCTION()
	uint8 GetEquipmentSlotByType(EEquipmentSlot EquipmentSlot);

	UFUNCTION()
	UDataTable* GetItemDB();

	UFUNCTION()
	void InitializePlayerAttributes();
	UPROPERTY()
	TArray<uint8> AttributesArray;

	UPROPERTY()
	AActor* CurrentContainer;
	
private:
	UPROPERTY()
	UDataTable* ItemDB;
	
	FReturnTupleBoolInt HasPartialStack(const FSlotStructure& ContentToAdd);
	
	UFUNCTION()
	bool CreateStack(FSlotStructure& ContentToAdd);
	UFUNCTION()
	bool AddToStack(FSlotStructure& ContentToAdd, const int8& Index);
	
	UFUNCTION()
	bool GetEmptyInventorySpace(uint8& OutIndex);
	UFUNCTION()
	EEquipmentSlot GetItemEquipmentSlot(FSlotStructure Item);
	UFUNCTION()
	bool ItemIsValid(FSlotStructure Item);
	UFUNCTION()
	bool GetEmptyEquipmentSlotByType(EEquipmentSlot EquipmentSlot, uint8& OutIndex);
	
	UFUNCTION()
	void UseEquipmentItem(const uint8& InventorySlot, const FSlotStructure& SlotStructure);

	UFUNCTION()
	void UseConsumableItem(const uint8& InventorySlot, FSlotStructure& InventoryItem);

	// Remove a specific amount from an existing item on Inventory
	UFUNCTION()
	void RemoveFromItemAmount(FSlotStructure& InventoryItem, const uint8& AmountToRemove, bool& WasFullAmountRemoved, uint8& AmountRemoved);

	// Remove Item From Inventory
	UFUNCTION()
	void RemoveItem(const uint8& InventorySlot);

	UFUNCTION()
	bool EquipItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot);
	
	UFUNCTION()
	bool UnEquipItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot);

	UFUNCTION()
	void DropItem(const uint8& InventorySlot);

	UFUNCTION(Category = "Manager|Private|Inventory")
	void MoveItem(UInventoryComponent* FromInventory, uint8 FromInventorySlot, UInventoryComponent* ToInventory, uint8 ToInventorySlot);
	UFUNCTION(Category = "Manager|Private|Inventory")
	void AddItem2(UInventoryComponent* Inventory, uint8 InventorySlot, FSlotStructure& InventoryItem);
	UFUNCTION(Category = "Manager|Private|Inventory")
	void RemoveItem2(UInventoryComponent* Inventory, uint8 InventorySlot);

	UFUNCTION(Category = "UserInterface|Private|Inventory")
	void ClearInventoryItem(const uint8& InventorySlot);
	UFUNCTION(Category = "UserInterface|Private|Inventory")
	void SetInventorySlotItem(const FSlotStructure& ContentToAdd, const uint8& InventorySlot);

	UFUNCTION(Category = "UserInterface|Private|Container") 
	void ClearContainerSlots();
	UFUNCTION(Category = "UserInterface|Private|Container")
	void CreateContainerSlots(uint8 NumberOfRows, uint8 SlotsPerRow);
	
	UFUNCTION(Category = "Manager|Private|Container")
	void SetContainerSlot(FSlotStructure ContentToAdd, uint8 InventorySlot);
	
	UFUNCTION(Category = "Manager|Private|Container")
	void OpenContainer(AActor* Container);
	UFUNCTION(Category = "Manager|Private|Container")
	void UseContainer(AActor* Container);
	
	UFUNCTION(Category = "Manager|Private|Container")
	void CloseContainer();
	UFUNCTION(Category = "Manager|Private|Container")
	void LoadContainerSlots(FContainerInfo ContainerProperties, const TArray<FSlotStructure>& InContainerInventory);
};