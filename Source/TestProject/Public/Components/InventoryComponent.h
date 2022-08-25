// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tuples.h"
#include "Components/ActorComponent.h"
#include "Inventory/EEquipmentSlot.h"
#include "Inventory/FSlotStructure.h"
#include "InventoryComponent.generated.h"

class UDataTable;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEMCPP_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="NumberOfRows", Category="Inventory UI"))
	uint8 NumberOfRowsInventory = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="SlotsPerRow", Category="Inventory UI"))
	uint8 SlotsPerRowInventory = 0;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Slots On Hotbar", Category = "Inventory UI"))
	uint8 NumberOfSlotsOnHotbar = 0;

	TArray<FSlotStructure> Inventory;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	UDataTable* ItemDB;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(Server, Reliable)
	virtual void Server_InitInventory(uint8 InventorySize);
	UFUNCTION()
	void InitInventory(uint8 InventorySize);

	UFUNCTION()
	bool LoadInventoryItems(uint8 Size, TArray<FSlotStructure> Array);

	UFUNCTION()
	virtual void ClearInventoryItem(uint8 InventorySlot);
	UFUNCTION()
	virtual void SetInventoryItem(uint8 InventorySlot, FSlotStructure& Item);

	UFUNCTION()
	FSlotStructure GetInventorySlot(uint8 InventorySlot);
	UFUNCTION()
	EEquipmentSlot GetEquipmentTypeBySlot(uint8 InventorySlot);
	
	UFUNCTION()
	void GetInventoryItems(TArray<FSlotStructure>& InventoryItems);

	/* If Item has Amount > 0 then is Valid and returns true, otherwise it is Invalid and returns false*/
	UFUNCTION()
	bool ItemIsValid(const FSlotStructure& Slot);

	FReturnTupleBoolInt GetEmptyInventorySpace();
	FReturnTupleBoolInt GetEmptyContainerSpace();
	bool GetEmptyInventorySpace(uint8& OutIndex);

	UFUNCTION()
	FSlotStructure GetInventoryItem(uint8 InventorySlot);
	
	UFUNCTION()
	void PrintInventory();

	UFUNCTION()
	FSlotStructure GetEmptySlot(EEquipmentSlot FromEquipmentType);
	UFUNCTION()
	FSlotStructure GetItemFromItemDB(FName Name);
};