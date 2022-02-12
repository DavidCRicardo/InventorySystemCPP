// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryInterface.h"
#include "Components/EquipmentComponent.h"
#include "Components/InventoryComponent.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API AMyPlayerController : public APlayerController, public IInventoryInterface
{
	GENERATED_BODY()
	
public:
	AMyPlayerController();

	virtual void UI_UseInventoryItem_Implementation(const uint8& InventorySlot) override;
	
	virtual void SetPawn(APawn* InPawn) override;

	FSlotStructure GetItemFrom(TArray<FSlotStructure> Inventory, const int8& SlotIndex);
	void AddItemToInventoryAndToIndex(TArray<FSlotStructure> Inventory, FSlotStructure& ContentToAdd, const uint8& InventorySlot);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(Category="Default", OverrideNativeName="InventoryComponent"))
	UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(Category="Default", OverrideNativeName="EquipmentComponent"))
	UEquipmentComponent* EquipmentComponent;

	
	UFUNCTION(BlueprintCallable, Category="Character")
	void ToggleProfile();
	
	UFUNCTION(BlueprintCallable, Category="Character")
	void ToggleInventory();

	UFUNCTION(BlueprintCallable, Category="Character")
	void ToggleMenu();

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void MoveInventoryItem(uint8 FromInventorySlot, uint8 ToInventorySlot);

	UFUNCTION()
	void RefreshWidgets();
	
protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable, Category="Character")
	void Interact();

	void PrintInventory();
	void PrintEquipment();

	UPROPERTY()
	class AMyHUD* HUD_Reference;
	
};