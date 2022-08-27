// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryHUDInterface.h"
#include "MyPlayerController.generated.h"

class UDataTable;
class AMyCharacter;
class UEquipmentComponent;
class UInventoryManagerComponent;
static const FName LCOMMON_WORDS = "/Game/UI/COMMON_WORDS.COMMON_WORDS";

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEMCPP_API AMyPlayerController : public APlayerController, public IInventoryHUDInterface
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
	virtual void UI_EquipFromContainer_Implementation(const uint8& FromInventorySlot, const uint8& ToInventorySlot) override;
	virtual void UI_UnEquipToContainer_Implementation(const uint8& FromInventorySlot, const uint8& ToInventorySlot) override;
	virtual void UI_MoveHotbarItem_Implementation(const uint8& FromSlot, const uint8& ToSlot, const bool IsDraggedFromInventory, const bool IsDraggedFromHotbar) override;
	/* Ends Interface */

	uint8 UIGetPlayerGold();

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
	UFUNCTION()
	void GetSelectedItemIndex(uint32& Index);

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
	
	uint8 GetMaximumHotbarSlots() { return MaximumHotbarSlots; };

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;
	
	UFUNCTION(BlueprintCallable, Category="Character")
	void Interact();
	
	UFUNCTION()
	void OnActorUsed(AActor* Actor1);
	
	UFUNCTION()
	void QuitGame();

	void UseHotbarSlot1();
	void UseHotbarSlot2();
	void UseHotbarSlot3();
	void UseHotbarSlot4();
	void UseHotbarSlot5();

private:
	uint8 MaximumHotbarSlots = 10;
};