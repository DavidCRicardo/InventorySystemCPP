// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tuples.h"
#include "Components/ActorComponent.h"
#include "Inventory/FSlotStructure.h"
#include "InventoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfSlots;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSlotStructure> Inventory;
	
	void InitInventory(int32 NumberSlots = 32);
	void InitializeInventoryLayout();

	UFUNCTION()
	void ToggleInventory();

	UFUNCTION()
	void RefreshInventoryUI();
	
	UFUNCTION()
	bool AddItem(FName ID, uint8 Amount);

	UFUNCTION()
	bool AddItemToInventory(FSlotStructure& ContentToAdd);

	UFUNCTION()
	FSlotStructure GetInventoryItem(uint8 InventorySlot);

	UFUNCTION()
	FSlotStructure GetEmptySlot();
	
	UFUNCTION()
	bool MoveInventoryItem(uint8 FromInventorySlot, uint8 ToInventorySlot);
	
	UFUNCTION()
	void AddItemToIndex(FSlotStructure& ContentToAdd, uint8 InventorySlot);

	UFUNCTION()
	void UseInventoryItem(const uint8& InventorySlot);

	
protected:
	UPROPERTY(EditAnywhere, Category = "Default")
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(EditAnywhere, Category = "Default")
	class UInventoryLayout* W_InventoryLayout;

private:
	UPROPERTY()
	UDataTable* ItemDB;
	
	FReturnTupleBoolInt HasPartialStack(const FSlotStructure& ContentToAdd);

	//UFUNCTION()
	//void HasPartialStack(const FSlotStructure& ContentToAdd, int8& OutIndex);
	
	UFUNCTION()
	bool CreateStack(FSlotStructure& ContentToAdd);
	UFUNCTION()
	bool AddToStack(FSlotStructure& ContentToAdd, const int8& Index);


	UFUNCTION()
	void UseConsumableItem(const uint8& InventorySlot, FSlotStructure& InventoryItem);

	UFUNCTION()
	void RemoveFromItemAmount(FSlotStructure& InventoryItem, const uint8& AmountToRemove, bool& WasFullAmountRemoved, uint8& AmountRemoved);

	UFUNCTION()
	void RemoveItem(TArray<FSlotStructure> OutInventory, const uint8& InventorySlot);
};