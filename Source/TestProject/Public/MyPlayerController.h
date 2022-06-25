// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryInterface.h"
#include "WorldActor.h"
#include "Components/EquipmentComponent.h"
#include "Components/InventoryManagerComponent.h"
#include "MyPlayerController.generated.h"

static const FName LCOMMON_WORDS = "/Game/UI/COMMON_WORDS.COMMON_WORDS";

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEMCPP_API AMyPlayerController : public APlayerController, public IInventoryInterface
{
	GENERATED_BODY()
	
public:
	AMyPlayerController();
	
	/* Interface */
	virtual void UI_MoveInventoryItem_Implementation(const uint8& FromInventorySlot, const uint8& ToInventorySlot) override;
	virtual void UI_DropInventoryItem_Implementation(const uint8& InventorySlot) override;
	virtual void UI_EquipInventoryItem_Implementation(const uint8& FromInventorySlot, const uint8& ToInventorySlot) override;
	virtual void UI_UnEquipInventoryItem_Implementation(const uint8& FromInventorySlot, const uint8& ToInventorySlot) override;
	virtual void UI_UseInventoryItem_Implementation(const uint8& InventorySlot) override;
	virtual void UI_UseContainerItem_Implementation(const uint8& InventorySlot) override;
	virtual void UI_TakeContainerItem_Implementation(const uint8& FromContainerSlot, const uint8& ToInventorySlot) override;
	virtual void UI_DepositContainerItem_Implementation(const uint8& FromInventorySlot, const uint8& ToInventorySlot) override;
	virtual void UI_MoveContainerItem_Implementation(const uint8& FromInventorySlot, const uint8& ToInventorySlot) override;
	/* Ends Interface */

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(Category="Inventory", OverrideNativeName="InventoryComponent"))
	UInventoryManagerComponent* InventoryManagerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(Category="Inventory", OverrideNativeName="EquipmentComponent"))
	UEquipmentComponent* PlayerInventoryComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Character Reference", Category="Default", OverrideNativeName="CharacterReference"))
	AMyCharacter* CharacterReference;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AMyHUD* HUD_Reference;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UHUDLayout* HUDLayoutReference;

	UFUNCTION()
	void SetupHUDReferences();

	UFUNCTION()
	TArray<uint8> GetPlayerAttributes();

	UFUNCTION()
	void RefreshContainerUI(uint8 SlotsPerSow, uint8 NumberOfRows);
	
	UFUNCTION()
	void CollectFromPanel(const FName& Name);

	UFUNCTION()
	bool IsContainerOpen();
	
	UFUNCTION(BlueprintCallable, Category="Character")
	void ToggleInventory();
	UFUNCTION(BlueprintCallable, Category="Character")
	void ToggleProfile();
	UFUNCTION(BlueprintCallable, Category="Character")
	void ToggleContainer();
	
	UFUNCTION(BlueprintCallable, Category="Character")
	void ToggleMenu();
	void GetSelectedItemIndex(uint32& Index);

	UFUNCTION()
	void RefreshWidgets();

	UFUNCTION(Server, Reliable)
	void Server_OnActorUsed(AActor* Actor);
	
	UFUNCTION()
	UUserWidget* GetInteractWidget();
	UFUNCTION()
	void SetMouseToCenterPosition();

	UFUNCTION(BlueprintCallable, Category="Character")
	void EnableUIMode();
	UFUNCTION(BlueprintCallable, Category="Character")
	void DisableUIMode();

	UFUNCTION()
	void AddUsableActorToDropMenu(FName IDName);
	UFUNCTION()
	void RemoveUsableActorToDropMenu(const FName& ID);

	UFUNCTION()
	UDataTable* GetItemDB();
protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable, Category="Character")
	void Interact();
	
	//UFUNCTION()
	//void PrintInventory();
	
	UFUNCTION()
	void QuitGame();

	UFUNCTION()
	void OnActorUsed(AActor* Actor);
};