// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentComponent.h"
#include "InventoryComponent.h"
#include "Tuples.h"
#include "Components/ActorComponent.h"
#include "Inventory/FSlotStructure.h"
#include "MyCharacter.h"
#include "UI/MainLayout.h"
#include "InventoryManagerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTPROJECT_API UInventoryManagerComponent : public UActorComponent
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

	
	UFUNCTION(Server, Reliable)
	void Server_EquipFromInventory(const uint8& FromInventorySlot, const uint8& ToInventorySlot);

	UFUNCTION(Server, Reliable)
	void Server_UnEquipFromInventory(const uint8& FromInventorySlot, const uint8& ToInventorySlot);
	
	UFUNCTION(Server, Reliable)
	void Server_DropItemFromInventory(const uint8& InventorySlot);

	UFUNCTION(Client, Reliable)
	void Client_SetInventorySlot(const FSlotStructure& ContentToAdd, const uint8& InventorySlot);
	
	UPROPERTY()
	AMyPlayerController* ControllerReference;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Player Inventory", Category="Default", OverrideNativeName="PlayerInventory"))
	UInventoryComponent* PlayerInventory;
	
	UFUNCTION()
	EEquipmentSlot GetEquipmentTypeBySlot(const uint8& EquipmentSlot);
	UFUNCTION()
	EItemType GetItemTypeBySlot(const uint8& ItemSlot);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfSlots;
	
	UPROPERTY(Replicated)
	uint8 InventorySize;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSlotStructure> Inventory;

	UPROPERTY()
	UMainLayout* InventoryUI;
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

	
	UFUNCTION(Category = "UserInterface|Private|Inventory")
	void ClearInventorySlot(const uint8& InventorySlot);
	UFUNCTION(Category = "UserInterface|Private|Inventory")
	void SetInventorySlot(const FSlotStructure& ContentToAdd, const uint8& InventorySlot);
};