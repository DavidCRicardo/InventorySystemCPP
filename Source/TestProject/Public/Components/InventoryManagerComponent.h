// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tuples.h"
#include "Components/ActorComponent.h"
#include "Inventory/FSlotStructure.h"
#include "MyCharacter.h"
#include "UI/InventoryLayout.h"
#include "InventoryManagerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTPROJECT_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void InitializeInventoryManagerUI(UInventoryLayout* Widget);

	UFUNCTION(Server, Reliable)
	void Server_InitInventory(uint8 InventorySize);
	
	UFUNCTION(Server, Reliable)
	void Server_EquipFromInventory(const uint8& FromInventorySlot, const uint8& ToInventorySlot);

	UFUNCTION(Server, Reliable)
	void Server_UnEquipFromInventory(const uint8& FromInventorySlot, const uint8& ToInventorySlot);
	
	UFUNCTION(Server, Reliable)
	void Server_DropItemFromInventory(const uint8& InventorySlot);
	
	UPROPERTY()
	AMyPlayerController* ControllerReference;
	
	UFUNCTION()
	EEquipmentSlot GetEquipmentTypeBySlot(const uint8& EquipmentSlot);
	UFUNCTION()
	EItemType GetItemTypeBySlot(const uint8& ItemSlot);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfSlots;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSlotStructure> Inventory;

	UPROPERTY()
	UInventoryLayout* InventoryUI;
	UPROPERTY()
	AMyCharacter* CharacterReference;
	
	virtual void InitInventory(int32 NumberSlots = 32);
	
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
	uint8 GetEquipmentSlotByType(EEquipmentSlot EquipmentSlot);
	
	UFUNCTION(Client, Reliable)
	void SetInventorySlotItem(uint32 InventorySlot, FSlotStructure SlotInformation);

	UFUNCTION(Client, Reliable)
	void Client_ClearAllInventorySlots();
	UFUNCTION(Category = "UserInterface|Private|Inventory")
	void ClearAllInventorySlots();
	
	UFUNCTION(Client, Reliable)
	void Client_ClearInventorySlot(const uint8& InventorySlot);
	UFUNCTION(Category = "UserInterface|Private|Inventory")
	void ClearInventorySlot(const uint8& InventorySlot);

	UFUNCTION(Client, Reliable)
	void Client_SetInventorySlot(const FSlotStructure& ContentToAdd, const uint8& InventorySlot);
	UFUNCTION(Category = "UserInterface|Private|Inventory")
	void SetInventorySlot(const FSlotStructure& ContentToAdd, const uint8& InventorySlot);

	UFUNCTION(Server, Reliable)
	void Server_RefreshInventorySlots();
	UFUNCTION(Category = "UserInterface|Private|Inventory")
	void RefreshInventorySlots();
private:
	UPROPERTY()
	UDataTable* ItemDB;
	
	FReturnTupleBoolInt HasPartialStack(const FSlotStructure& ContentToAdd);
	
	UFUNCTION()
	bool CreateStack(FSlotStructure& ContentToAdd);
	UFUNCTION()
	bool AddToStack(FSlotStructure& ContentToAdd, const int8& Index);

	UFUNCTION()
	void UseConsumableItem(const uint8& InventorySlot, FSlotStructure& InventoryItem);

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
	void RemoveFromItemAmount(FSlotStructure& InventoryItem, const uint8& AmountToRemove, bool& WasFullAmountRemoved, uint8& AmountRemoved);

	UFUNCTION()
	void RemoveItem(const uint8& InventorySlot);
	UFUNCTION()
	bool EquipItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot);
	
	UFUNCTION()
	bool UnEquipItem(const uint8& FromInventorySlot, const uint8& ToInventorySlot);

	UFUNCTION()
	void DropItem(const uint8& InventorySlot);
};