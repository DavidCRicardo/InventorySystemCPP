// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryInterface.h"
#include "Components/EquipmentComponent.h"
#include "Components/InventoryManagerComponent.h"
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

	/* Interface */
	virtual void UI_UseInventoryItem_Implementation(const uint8& InventorySlot) override;
	virtual void UI_MoveInventoryItem_Implementation(const uint8& FromInventorySlot, const uint8& ToInventorySlot) override;
	virtual void UI_DropInventoryItem_Implementation(const uint8& InventorySlot) override;
	virtual void UI_EquipInventoryItem_Implementation(const uint8& FromInventorySlot, const uint8& ToInventorySlot) override;
	virtual void UI_UnEquipInventoryItem_Implementation(const uint8& FromInventorySlot, const uint8& ToInventorySlot) override;
	/* Ends Interface */
	
	virtual void SetPawn(APawn* InPawn) override;

	FSlotStructure GetItemFrom(TArray<FSlotStructure> Inventory, const int8& SlotIndex);
	
	void AddItemToInventoryAndToIndex(TArray<FSlotStructure> Inventory, FSlotStructure& ContentToAdd, const uint8& InventorySlot);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AMyHUD* HUD_Reference;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(Category="Default", OverrideNativeName="InventoryComponent"))
	UInventoryManagerComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(Category="Default", OverrideNativeName="EquipmentComponent"))
	UEquipmentComponent* EquipmentComponent;
	
	UFUNCTION(BlueprintCallable, Category="Character")
	void ToggleProfile();
	
	UFUNCTION(BlueprintCallable, Category="Character")
	void ToggleInventory();

	UFUNCTION(BlueprintCallable, Category="Character")
	void ToggleMenu();
	
	UFUNCTION()
	void RefreshWidgets();
	
	// UFUNCTION(Client)
	// void Client_GetUsableActorFocus();
	UFUNCTION()
	void GetUsableActorFocus();

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	UPROPERTY(VisibleAnywhere, Category="Character")
	AMyCharacter* CharacterReference;

	UFUNCTION(BlueprintCallable, Category="Character")
	void Interact();

	void PrintInventory();
	//void PrintEquipment();
	
};